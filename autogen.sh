#!/bin/sh
# Run this program to build the configuration
# files for GNU Typist:
# [*/]Makefile.in, aclocal.m4, configure,
# config.h.in, config.sub, config.guess
# INSTALL, configur.bat, doc/gtypist.info, lessons/gtypist.typ (maybe more)

# warning: if you use autoconf 2.50 or later, then you need to use
# gettext 0.10.39 or above and not 0.10.38 (earlier versions _might_ work ?)


(autoconf --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`autoconf' installed to."
    echo "Download the appropriate package for your distribution,"
    echo "or get the source tarball at ftp://ftp.gnu.org/pub/gnu/"
    exit 1
}

(automake --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`automake' installed."
    echo "Get ftp://ftp.gnu.org/pub/gnu/automake-1.4.tar.gz"
    echo "(or a newer version if it is available)"
    exit 1
}

(gettext --version) < /dev/null > /dev/null 2>&1 || {
    echo
    echo "**Error**: You must have \`gettext' installed."
    echo "Get ftp://ftp.gnu.org/gnu/gettext-0.10.39.tar.gz"
    echo "(or a newer version if it is available)"
    exit 1
}

echo "creating doc/gtypist.info..."
makeinfo -Idoc doc/gtypist.texi -o doc/gtypist.info
echo "creating lessons/gtypist.typ..."
(cd lessons && ../tools/typcombine q.typ r.typ t.typ v.typ u.typ d.typ m.typ s.typ n.typ > gtypist.typ)


# Get version and write it in files
. version.sh

# make sure that there's no whitespace after the version number
# (in that case the awk command at the top of configure.in won't work)
VERSION_FROM_AWK="`cat version.sh | grep '^VERSION' | awk -F= '{print $2}'`"
if test "$VERSION_FROM_AWK" != "$VERSION"; then
    echo "There is whitespace around the version-number in version.sh. Please fix it."
    exit -1
fi

for file in configur.bat INSTALL
  do
  echo "creating $file..."
  if test $file -nt ${file}.in; then
      echo "*** \"$file\" has been modified."
      echo "\"$file\" is generated from ${file}.in."
      echo "Please apply the changes to ${file}.in instead."
      exit -1;
  fi
  sed "s/@VERSION/$VERSION/g" ${file}.in > $file 
  # TODO: this causes cvs to think that ${file}.in was modified...
  touch ${file}.in
done

# gettextize
echo "running gettextize...  Ignore non-fatal messages."
gettextize --force --copy

# Build configuration files

echo "creating build configuration files..."
rm -f config.cache
aclocal
autoheader
automake --add-missing
autoconf

if test -z "$*"; then
    echo
    echo "**Warning**: I am going to run \`configure' with no arguments."
    echo "If you wish to pass any to it, please specify them on the"
    echo \`$0\'" command line."
fi

echo
echo running ./configure "$@"...
echo
./configure "$@" && echo Type \`make\' to compile gtypist
