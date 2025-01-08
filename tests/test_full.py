import unittest
from pathlib import Path


class TestFull(unittest.TestCase):

    def setUp(self):
        self.RESOURCES = Path('tests/resources')

    def test_full(self):
        self.assertTrue(True)
