#!/usr/bin/env python3
"""Behavior checks for every exercise, using only the Python standard library."""
from pathlib import Path
import itertools
import os
import signal
import struct
import subprocess
import sys
import tempfile
import unittest

if len(sys.argv) != 2:
    raise SystemExit("Usage: python3 tests/check.py BUILD_DIRECTORY")
BUILD = Path(sys.argv.pop()).resolve()
ROOT = Path(__file__).resolve().parents[1]


class Exercises(unittest.TestCase):
    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory(prefix="systems-exercises-")
        self.directory = Path(self.scratch.name)
        self.addCleanup(self.scratch.cleanup)

    def run_program(self, name, *arguments, data=b"", success=True):
        # Kill the whole process group on timeout, including forked children.
        process = subprocess.Popen(
            [str(BUILD / name), *map(str, arguments)],
            stdin=subprocess.PIPE, stdout=subprocess.PIPE, stderr=subprocess.PIPE,
            cwd=self.directory, start_new_session=True)
        try:
            stdout, stderr = process.communicate(data, timeout=15)
        except subprocess.TimeoutExpired:
            os.killpg(process.pid, signal.SIGKILL)
            process.communicate()
            self.fail(f"{name} exceeded its timeout")
        if success:
            self.assertEqual(process.returncode, 0, stderr.decode(errors="replace"))
        else:
            self.assertNotEqual(process.returncode, 0, stdout.decode(errors="replace"))
        return stdout

    def records(self):
        text = self.directory / "students.txt"
        binary = self.directory / "students.bin"
        text.write_bytes((ROOT / "examples/students.txt").read_bytes())
        self.run_program("record-converter", "to-binary", text, binary)
        return text, binary

    def sort_input(self, filename, values):
        path = self.directory / filename
        path.write_bytes(struct.pack("<I", len(values)) +
                         b"".join(struct.pack("<i", x) for x in values))
        return path

    def test_record_round_trip_and_layout(self):
        text, binary = self.records()
        expected = b"".join(struct.pack("<II31s31sI", *row) for row in (
            (1, 100001, b"Sample", b"Alice", 25),
            (2, 100002, b"Example", b"Bob", 22),
            (3, 100003, b"Demo", b"Carol", 30)))
        self.assertEqual(binary.read_bytes(), expected)
        output = self.directory / "again.txt"
        self.run_program("record-converter", "to-text", binary, output)
        self.assertEqual(output.read_bytes(), text.read_bytes())

    def test_record_empty_files(self):
        text = self.directory / "empty.txt"
        binary = self.directory / "empty.bin"
        output = self.directory / "again.txt"
        text.touch()
        self.run_program("record-converter", "to-binary", text, binary)
        self.run_program("record-converter", "to-text", binary, output)
        self.assertEqual(binary.read_bytes(), b"")
        self.assertEqual(output.read_bytes(), b"")

    def test_record_invalid_text(self):
        for line in (b"1 100001 Sample Alice 31\n",
                     b"1 100001 " + b"x" * 31 + b" Alice 20\n",
                     b"2 100001 Sample Alice 20\n", b"bad data\n", b"999999999999999999999999999999 100001 Sample Alice 20\n",
                     b"1 100001 Sample Alice 20 extra\n",
                     b"x" * 300 + b"\n"):
            text = self.directory / "bad.txt"
            text.write_bytes(line)
            self.run_program("record-converter", "to-binary", text,
                             self.directory / "bad.bin", success=False)

    def test_record_truncation_and_missing_terminator(self):
        _, binary = self.records()
        original = binary.read_bytes()
        output = self.directory / "bad.txt"
        for damaged in (original[:-1], original[:8] + b"x" * 31 + original[39:]):
            binary.write_bytes(damaged)
            self.run_program("record-converter", "to-text", binary, output, success=False)

    def test_record_same_file_and_hardlink_preserve_input(self):
        text = self.directory / "original.txt"
        content = b"1 100001 Sample Alice 25\n"
        text.write_bytes(content)
        self.run_program("record-converter", "to-binary", text, text, success=False)
        alias = self.directory / "alias.txt"
        alias.hardlink_to(text)
        self.run_program("record-converter", "to-binary", text, alias, success=False)
        self.assertEqual(text.read_bytes(), content)

    def test_record_random_access_update_and_append(self):
        _, binary = self.records()
        self.assertEqual(self.run_program("record-access", "read", binary, 2),
                         b"2 100002 Example Bob 22\n")
        self.run_program("record-access", "write", binary, 2, 100002, "Example", "Bob", 28)
        self.run_program("record-access", "write", binary, 4, 100004, "Test", "Dana", 30)
        self.assertEqual(self.run_program("record-access", "read", binary, 2),
                         b"2 100002 Example Bob 28\n")
        self.assertEqual(self.run_program("record-access", "read", binary, 4),
                         b"4 100004 Test Dana 30\n")
        self.assertEqual(binary.stat().st_size, 4 * 74)

    def test_record_invalid_index_and_sparse_write(self):
        _, binary = self.records()
        original = binary.read_bytes()
        for index in ("0", "-1", "4", "abc"):
            self.run_program("record-access", "read", binary, index, success=False)
        self.run_program("record-access", "write", binary, 5, 100005,
                         "Gap", "Test", 20, success=False)
        self.assertEqual(binary.read_bytes(), original)

    def test_matrix_matches_sequential_reference(self):
        for rows, inner, cols in ((1, 1, 1), (2, 2, 3), (3, 4, 2), (4, 3, 4)):
            actual = self.run_program("pthread-matrix", rows, inner, cols).decode()
            expected = [[sum((i * inner + k + 1) * (k * cols + j + 1)
                             for k in range(inner)) for j in range(cols)]
                        for i in range(rows)]
            self.assertEqual([[int(x) for x in row.split()]
                              for row in actual.splitlines()], expected)

    def test_thread_tree_complete_leaf_paths(self):
        for depth in (0, 1, 3, 5):
            leaves = self.run_program("thread-tree", depth).decode().splitlines()
            expected = ["".join(path) for path in itertools.product("LR", repeat=depth)]
            self.assertEqual(sorted(leaves), sorted(expected if depth else ["root"]))

    def test_process_tree_complete_leaf_paths(self):
        for height, branches in ((0, 2), (1, 3), (2, 2), (3, 2)):
            leaves = self.run_program("process-tree", height, branches).decode().splitlines()
            expected = [".".join(map(str, path))
                        for path in itertools.product(range(branches), repeat=height)]
            self.assertEqual(sorted(leaves), sorted(expected if height else ["root"]))

    def test_signal_order_repeated_runs(self):
        expected = b"".join(f"PARENT {i}\nCHILD {i}\n".encode() for i in range(1, 21))
        for _ in range(5):
            self.assertEqual(self.run_program("signal-handshake", 20), expected)

    def test_pipe_echo_empty_text_and_binary_streams(self):
        for content in (b"", b"first\nsecond\n", bytes(range(256)) * 1024):
            self.assertEqual(self.run_program("pipe-echo", data=content), content)

    def test_file_sort_exact_output(self):
        runs = ([23, 1, -5, 23], [0, 90, -2147483648, 2147483647], [])
        inputs = [self.sort_input(f"input{i}.bin", run) for i, run in enumerate(runs)]
        output = self.directory / "sorted.bin"
        self.run_program("file-sort", *inputs, output)
        expected = sorted(itertools.chain.from_iterable(runs))
        self.assertEqual(output.read_bytes(), struct.pack("<I", len(expected)) +
                         b"".join(struct.pack("<i", x) for x in expected))

    def test_file_sort_rejects_missing_truncated_or_extra_data(self):
        good = self.sort_input("good.bin", [1])
        bad = self.directory / "bad.bin"
        output = self.directory / "sorted.bin"
        for data in (b"", struct.pack("<I", 2) + struct.pack("<i", 1),
                     struct.pack("<I", 0) + b"extra", struct.pack("<I", 1000001)):
            bad.write_bytes(data)
            self.run_program("file-sort", good, bad, output, success=False)
            self.assertFalse(output.exists())
        self.run_program("file-sort", self.directory / "missing.bin", output, success=False)

    def test_future_pipeline_values_and_overflow_boundary(self):
        for value, expected in ((-3, False), (0, False), (1, False), (2, True),
                                (9, False), (17, True), (2147483647, True)):
            self.assertEqual(self.run_program("future-pipeline", value),
                             b"prime\n" if expected else b"not prime\n")
        for value in ("abc", "12x", "2147483648", ""):
            self.run_program("future-pipeline", value, success=False)

    def test_semaphore_order_repeated_runs(self):
        for _ in range(5):
            self.assertEqual(self.run_program("semaphore-sequencing", 10, 3),
                             b"+++\n---\n***\n" * 10)

    def test_template_containers(self):
        self.assertEqual(self.run_program("template-containers"),
                         b"stack: third second first\nqueue: 1 2 3\n")
        self.run_program("test-containers")

    def test_invalid_arguments(self):
        for name, arguments in (
            ("record-converter", ()), ("record-access", ()),
            ("pthread-matrix", (32, 32, 32)), ("pthread-matrix", ("-1", 2, 2)),
            ("thread-tree", (7,)), ("process-tree", (5, 2)),
            ("process-tree", (2, 0)), ("signal-handshake", (0,)),
            ("semaphore-sequencing", (0, 5)), ("file-sort", ()),
            ("future-pipeline", ()), ("pipe-echo", ("unexpected",))):
            self.run_program(name, *arguments, success=False)


if __name__ == "__main__":
    unittest.main(verbosity=2)
