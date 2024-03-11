import gdb
from subprocess import CalledProcessError, check_output

lib = "sar.so"
process_names = ["chaos", "revolution"]

"""Load SAR into a process"""
class sar_load(gdb.Command):
    def __init__(self):
        super(sar_load, self).__init__("sar_load", gdb.COMMAND_USER)

    def invoke(self, argument, fromtty):
        argv = gdb.string_to_argv(argument)
        if len(argv) == 1:
            if argv[0] not in process_names:
                print("invalid process name")
                return

            try:
                pid = check_output(["pidof", argv[0]], encoding='UTF-8').strip()
            except CalledProcessError as err:
                print(err)
                return

            gdb.execute(f"attach {pid}")
            gdb.execute("set $dlopen = (void*(*)(char*, int)) dlopen")
            gdb.execute("set $dlsym = (void*(*)(void*, char*)) dlsym")
            gdb.execute(f"set $library = $dlopen(\"{lib}\", 1)")
            gdb.execute("set $sar_load = (int(*)()) $dlsym($library, \"sar_load\")")
            gdb.execute("call $sar_load()")
            gdb.execute("continue")
        else:
            print("sar_load <process_name>")

"""Unload SAR from a process"""
class sar_unload(gdb.Command):
    def __init__(self):
        super(sar_unload, self).__init__("sar_unload", gdb.COMMAND_USER)

    def invoke(self, argument, fromtty):
        argv = gdb.string_to_argv(argument)
        if len(argv) == 1:
            if argv[0] not in process_names:
                print("invalid process name")
                return

            try:
                pid = check_output(["pidof", argv[0]], encoding='UTF-8').strip()
            except CalledProcessError as err:
                print(err)
                return

            gdb.execute("set $dlopen = (void*(*)(char*, int)) dlopen")
            gdb.execute("set $dlsym = (void*(*)(void*, char*)) dlsym")
            gdb.execute("set $dlclose = (int(*)(void*)) dlclose")
            gdb.execute(f"set $library = $dlopen(\"{lib}\", 6)")
            gdb.execute("set $sar_unload = (void(*)()) $dlsym($library, \"sar_unload\")")
            gdb.execute("call $sar_unload()")
            gdb.execute("call $dlclose($library)")
            gdb.execute("call $dlclose($library)")
            gdb.execute("continue")
        else:
            print("sar_unload <process_name>")

sar_load()
sar_unload()
