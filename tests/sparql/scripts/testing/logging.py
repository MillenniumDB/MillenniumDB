import sys
from enum import Enum
from typing import Dict, List

import colorama
from tqdm import tqdm

from . import options
from .options import LOGGING_LEVELS

colorama.init(autoreset=True, strip=False)


class Level(Enum):
    CORRECT = 1
    ERROR = 2
    BEGIN = 3
    END = 4
    WARNING = 5
    SKIPPED = 6
    OUTPUT = 7
    SUMMARY = 8
    DEBUG = 9
    SERVER_LOG = 10


for level in LOGGING_LEVELS:
    if not hasattr(Level, level):
        print(f'Invalid logging level "{level}" specified in {options.__name__}')
        print("Available levels:", ", ".join(v.name for v in Level))
        sys.exit(1)


__PREFIX: Dict[Enum, str] = {
    Level.CORRECT: colorama.Fore.GREEN + "[CORRECT]" + colorama.Fore.RESET,
    Level.ERROR: colorama.Fore.RED + "[ERROR]" + colorama.Fore.RESET,
    Level.BEGIN: colorama.Fore.WHITE + "[BEGIN]" + colorama.Fore.RESET,
    Level.END: colorama.Fore.WHITE + "[END]" + colorama.Fore.RESET,
    Level.WARNING: colorama.Fore.YELLOW + "[WARNING]" + colorama.Fore.RESET,
    Level.SKIPPED: colorama.Fore.BLUE + "[SKIPPED]" + colorama.Fore.RESET,
    Level.OUTPUT: colorama.Fore.CYAN + "[OUTPUT]" + colorama.Fore.RESET,
    Level.SUMMARY: colorama.Fore.YELLOW + "[SUMMARY]" + colorama.Fore.RESET,
    Level.DEBUG: colorama.Fore.CYAN + "[DEBUG]" + colorama.Fore.RESET,
    Level.SERVER_LOG: colorama.Fore.CYAN + "[SERVER LOG]" + colorama.Fore.RESET,
}


__summary_buffer: List[str] = []


def print_summary():
    global __summary_buffer

    for element in __summary_buffer:
        print(element)
    __summary_buffer = []


def log(level: Level, *msgs: str):
    if not LOGGING_LEVELS[level.name]:
        return

    prefix = __PREFIX[level]
    msg_string: str = " ".join([prefix, *msgs])

    if level == Level.SUMMARY:
        __summary_buffer.append(msg_string)
    else:
        tqdm.write(msg_string)
