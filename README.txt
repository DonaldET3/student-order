Student Ordering tool
version 1

A program for ordering students based on several different factors.

command line arguments
h: help message
f: load data from a file

the internal command prompt is: so>

core program commands
quit, q: quit the program
help, h, ?: help message
filehelp, fh: list file commands
studenthelp, sh: list student commands
rulehelp, rh: list ordering rule commands

file commands
store, save, s: store data in a file
load, l: load data from a file
clear, reset, new: clear program data

student commands
addstudent, as: add a student to the data set
editstudent, es: edit student information
removestudent, rs: remove a student from the data set
liststudents, ls: display a list of students

rule commands
ampm: set morning or afternoon stop ordering
gradeprecedence, ap: precedence of grade order in seating arrangement
genderprecedence, gp: precedence of gender in seating assignment
stopprecedence, sp: precedence of stop order in seating arrangement
viewruleorder, vro: display student ordering rule configuraton
vieworder, vo: output ordered student list

All of the data can be stored and loaded again later.

student data includes: name, grade, gender, stop number
ordering rules are: by grade, gender zoning, and stop number
ordering rules are assigned a precedence
student ordering by stop is reversed depending on whether it's a morning or afternoon chart
