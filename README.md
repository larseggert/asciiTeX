# You should be using [utftex](https://github.com/bartp5/libtexprintf)!

It is actively supported and produces better output than this.

---

# asciiTeX

asciiTeX is an ASCII equation renderer released under the terms of the GNU
General Public License. The program can generate ASCII art representations of
mathematical equations. You can use asciiTeX to quickly insert equations in,
e.g., emails or comments in your source code. The syntax is similar to LaTex.
The asciiTeX project is a fork of eqascii, providing new features and many bug
fixes to the original program.

## Changes

This is a fork of [Bart Pieters' asciiTeX](http://asciitex.sourceforge.net/)
that focuses on improving compatibility with
[tex2svg](https://github.com/Ximik/tex2svg), for use with
[kramdown-rfc2629](https://github.com/cabo/kramdown-rfc2629) to produce prettier
math for [IETF](https://ietf.org/) specifications.

Specifically, these changes were made (so far):

- Unicode support
- support for `<>` braces
- support for `\eft\` and `\right`
- support for `\text` and `\mathrm`
- support for `\binom`
- better automatic whitespace insertion/removal
- remove trailing whitespaces from output
- switch build system to cmake
- removal of the GUI
- fix numerous minor bugs

There are very likely many [tex2svg](https://github.com/Ximik/tex2svg) features
still missing from this fork of asciiTeX. There are also very likely still many
remaining bugs. I'd be happy to see issues filed and pull-requests opened to
further improve this software.

## Installing from sources

~~~ shell
git clone https://github.com/larseggert/asciiTeX.git
cd asciiTeX
mkdir build
cd build
cmake ..
cmake --build .
~~~
