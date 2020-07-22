#!/usr/bin/env python3

import argparse
import glob
import json
import logging
import os
import pathlib

from fnmatch import fnmatch
from shutil import copy
from subprocess import call


class ClangFormat:

    def __init__(self, path):
        """ Init class to process clang format.

        :param path: Path to repository where clang-format should started. In this directory or one folder up, must be
        placed .clang-format file.
        :param clang_ignore: File with list of directories and files which should be ignored in formatting.
        """
        self.path = path
        
        clang_config = os.path.join(pathlib.Path(__file__).parent.absolute(), ".clang.config")
        
        with open(clang_config, "r") as f:
            self.config = json.load(f)

        self.cmd = [self.config["clang_cmd"], "-i", "-verbose", "-style=file"]
        if not os.path.isfile("{}/.clang-format".format(self.path)):
            logging.warning(" Missing .clang-format file in root repository directory")
            logging.info(" Copying .clang-format from dev-tools repository")
            copy("{}/.clang-format".format(os.path.dirname(os.path.abspath(__file__))), self.path)

    def get_list_of_files(self) -> list:
        """ Function to get list of whole files in repository. With defined extensions in .clang.config file.

        :return: List of whole files in project.
        """

        files = []
        for value in self.config["extensions"]:
            files.extend(glob.glob("{}/**/{}".format(self.path, value), recursive=True))

        return files

    def exclude_ignored_paths_and_dirs(self, files: list) -> list:
        """ Function to exclude ignored directories and files from --clang_ignore file argument.

        :param files: List of whole files in project.
        :return: List of files to format.
        """
        whole_files = files
        ignored = []

        for file_path in files:
            for excluded in self.config["ignore_directories"]:
                if fnmatch(file_path, excluded):
                    ignored.append(file_path)
                    break

        files_to_format = [x for x in whole_files if x not in ignored]
        logging.debug("Files to format :\n {}".format("\n".join(files_to_format)))
        return files_to_format

    def format_files(self, files_to_format: list) -> None:
        """ Function to format files.

        :param files_to_format: List of files to format.
        :return: None
        """
        cmd = self.cmd
        cmd.append("")
        cmd_len = len(cmd)

        for file in files_to_format:
            cmd[cmd_len - 1] = file
            ret = call(cmd)

            if ret is not 0:
                logging.error("Clang format error {} for {}".format(ret, cmd))


if __name__ == '__main__':

    logging.basicConfig(format='%(levelname)s:%(message)s', level=logging.DEBUG)

    parser = argparse.ArgumentParser(description="Format project with clang",  formatter_class=argparse.RawTextHelpFormatter)

    parser.add_argument("--path", help="Path to repository with files to format.", required=True)

    args = vars(parser.parse_args())

    cf = ClangFormat(args["path"])
    file_list = cf.get_list_of_files()
    cf.format_files(cf.exclude_ignored_paths_and_dirs(file_list))
