#!/usr/bin/perl
use File::Copy;

foreach $file (@ARGV)
{
    open (INFILE, "<$file");
    open (OUTFILE, ">${file}_tmp");
    while (my $line = <INFILE>)
    {
        if ($line =~ /^ \* Copyright \(c\).*Jeremy Erickson$/)
        {
            print OUTFILE " * Copyright (c) 2005-2009 Jeremy Erickson\n";
        }
        else
        {
            print OUTFILE $line;
        }
    }
    close INFILE;
    close OUTFILE;
    move("${file}_tmp", "$file");
}
