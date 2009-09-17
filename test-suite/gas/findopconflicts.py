#!/usr/bin/python

import glob, os, re

def readasm(filename):
  opcodes = []
  f = open(filename, "r")
  file = f.read()
  f.close()
  for line in file.split("\n"):
    line = line.strip()
    if line and line[0] != "#" and line[-1] != ":":
      opcode = line.split(" ")[0]
      opcodes.append([opcode, line])
  return opcodes

def readdiasm(filename):
  lines = []
  f = open(filename, "r")
  file = f.read()
  f.close()
  return file.split("\n")

if __name__ == "__main__":

  files = glob.glob("*.s")
  for filename in files:
    Opcodes = readasm(filename)
    DisasmLines = readdiasm(filename.replace(".s", ".d"))
    print "Confliting opcodes in %s..." % filename.replace(".s", "")
    LineCount = 0
    for op, insn in Opcodes:
      regex = re.compile(op)
      res = regex.search(DisasmLines[LineCount])
      if not res:
        print "Looking for \"%s\", but found:" % insn
        print DisasmLines[LineCount]
      LineCount += 1
