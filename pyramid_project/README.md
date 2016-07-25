This directory contains the code I wrote for a graduate project
in my Computer Security course at the University of Rochester.

It was based on an idea I had that sprung from a 2013 paper by
Williams and Sion, "Access Privacy and Correctness on Untrusted
Storage".  It attempts to implement Oblivious RAM (ORAM), in
which an attacker snooping on the requests made to a storage
medium cannot determine what information is being accessed.  This
is accomplished by encrypting all blocks of data before they are
written to disk and by never writing a specific block of data to
the same physical address on consecutive accesses.

These blocks are stored in a pyramidal structure, in which each
level i, starting from i=1, may store up to 4^i blocks.  When a
block is written to disk, it is placed at the top of the pyramid
if that level has room for it.  If the level is full, its contents
are dumped into the next level below it.

Accessing a block within the pyramid proceeds as follows.  When a
block is requested, each level of the pyramid is checked to see if
it contains said block.  If so, that block is accessed.  If not, a
randomly selected block from that level is accessed.

The goal of my project was to apply these ideas to accessing
information within a file rather than on physical media.  I
imagined a file being divided into a series of 4KB blocks, and I
wrote wrapper functions pyramid_open, pyramid_close, pyramid_read,
and pyramid_write that would correctly access the file as
requested while performing all necessary encryption, decryption,
and pyramid operations.

Unfortunately, my project didn't achieve the level of security I
had hoped for.  The problem was that, although I was obfuscating
disk access requests with a number of phony block accesses, it
would not conceal sequential file reading.  For example, suppose a
user wants to read blocks 3, 4, and 5 of a file, and that the
pyramid contains 3 levels.  In this case, the list of accesses
might be something like this: 34, 23, 3, 11, 4, 10, 22, 67, 5.
If an attacker knows how large the file is, they can determine how
many levels the pyramid contains, and they can look at every group
of 3 accesses and know that at least one of those blocks was
accessed.  They would be able to notice that block 3 was accessed
in the first group, that block 4 was accessed in the second group,
and that block 5 was accessed in the third group.  From this
information, it would be easy to deduce that it was likely that
the user requested blocks 3, 4, and 5 sequentially, due to the
pattern.  This wasn't a problem with proper ORAM because sequential
logical blocks are not necessarily stored adjacent to each other in
physical media, but blocks within a file are always thought of as
being next to each other.  Dissociating the principles of ORAM from
the complexities of physical media and abstracting away the means
by which information is written to disk makes the methods of ORAM
ineffective.

Although this software did not accomplish what I set out to do, I
include it in my portfolio as an example of a larger project I
worked on, and as an example of how I organize large numbers of
individual pieces of code.
