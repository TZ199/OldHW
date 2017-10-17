# Notes:
# $@ means "The file name of the target of the rule"
# $< means "The name of the first prerequisite"
# $+ means "The names of all the prerequisites, with spaces between them, exactly as given"
# For more magic automatic variables, see
# <http://www.gnu.org/software/make/manual/html_chapter/make_10.html#SEC111>

# First target is "run", which will cause Make build and run the specified target
# The prerequisite should be the Java program name to run (without the ".class" suffix)

run: TestResolve

CLASSES = \
	TestRegister.class \
	TestRegisterWithAttributes.class \
	TestRegisterWithUpdates.class \
	TestBrowse.class \
	TestResolve.class \
	TestResolveWithMonitoring.class

all: $(CLASSES)

clean:
	rm $(CLASSES)

# Rules to run programs with appropriate arguments

TestRegister: TestRegister.class
	java -Dfile.encoding=UTF8 $@ "Moët & Chandon"

TestRegisterWithAttributes: TestRegisterWithAttributes.class
	java -Dfile.encoding=UTF8 $@ "Moët & Chandon"

TestRegisterWithUpdates: TestRegisterWithUpdates.class
	java -Dfile.encoding=UTF8 $@ "Moët & Chandon"

TestResolve: TestResolve.class
	java -Dfile.encoding=UTF8 $@ "Moët & Chandon" local

TestResolveWithMonitoring: TestResolveWithMonitoring.class
	java -Dfile.encoding=UTF8 $@ "Moët & Chandon" local

dns-sd:
	dns-sd -Q 'Moët\032&\032Chandon._example._tcp.local.' 16

# Implicit rules

%.class: %.java
	javac -encoding UTF8 $<

%: %.class
	java -Dfile.encoding=UTF8 $@
