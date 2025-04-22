#The following tests check whether the parsing of parse_print works correctly or not

#imports

from argparse import ArgumentParser
import subprocess as sup
import pytest

#-------------------------------------------------------------------TESTS FOR SIMPLE COMMANDS----------------------------------------------------

class Test_simple_command:

    #TEST_1
    def test_basic_echo(self):
        expected = 'FALSE "echo hello" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_2
    def test_basic_echo_two_args(self):
        expected = 'FALSE "echo hello world" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello world"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_3
    def test_basic_no_args(self):
        expected = 'FALSE "ls" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"ls"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_4
    def test_basic_no_command_no_args(self):
        expected = 'FALSE "" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '""'], capture_output=True, text=True)
        assert expected == proc.stdout

#----------------------------------------------------------------------TESTS FOR COMMAND LISTS-----------------------------------------------------

class Test_command_lists:

    #TEST_1
    def test_two_commands(self):
        expected = 'FALSE "echo hello FALSE echo world" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello; echo world"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_2
    def test_multiple_commands(self):
        expected = 'FALSE "echo hello FALSE echo world FALSE cat file_1 FALSE ls FALSE echo ciao" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello; echo world; cat file_1; ls; echo ciao"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_3
    def test_multiple_commands_args(self):
        expected = 'FALSE "echo hello world FALSE cat file1 file2 file3 file4 file5 file6 FALSE ls FALSE chmod u+x file1 file2 file3" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello world ; cat file1 file2 file3 file4 file5 file6; ls; chmod u+x file1 file2 file3"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_5
    #def test_unexpected_token_one(self):
    #    expected = '"parse_print: Unexpected token" \n'
    #    proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello world;;"'], capture_output=True, text=True)
    #    assert expected == proc.stdout

    #TEST_6
    #def test_unexpected_token_two(self):
    #    expected = '"parse_print: Unexpected token" \n'
    #    proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello world; cat file1 file2 file3; ls;;"'], capture_output=True, text=True)
    #    assert expected == proc.stdout

#----------------------------------------------------------------------TESTS FOR PIPELINE-----------------------------------------------------

class Test_pipelines:

    #TEST_1
    def test_pipelines_single(self):
        expected = 'FALSE "echo hello  | echo hi" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello | echo hi"'], capture_output=True, text=True)
        assert expected == proc.stdout

    #TEST_2
    def test_pipelines_multiple(self):
        expected = 'FALSE "echo hello world  | echo hi FALSE ls  | echo you" \n'
        proc = sup.run(['../../builddir/parse_print', '-c', '"echo hello world | echo hi ; ls | echo you"'], capture_output=True, text=True)
