#!/usr/bin/perl
if (@ARGV == 0)
{
    die "Requires one parameter, Unix install prefix.\n";
}
my $inst_dir = $ARGV[0];
my $bundle = "$inst_dir/BibleMemorizer.app";
my $plugin_dir = "$inst_dir/Plugins";

my @plugins = `/bin/ls $plugin_dir/*.plugin`;
foreach $plugin (@plugins)
{
    chomp $plugin;
    open INFILE, "<$plugin";
    open OUTFILE, ">${plugin}_tmp";
    while (my $line = <INFILE>)
    {
        if ($line =~ /^FileName\=/ && $line !~ /\.dylib$/)
        {
            chomp $line;
            print OUTFILE "$line.dylib\n";
        }
        else
        {
            print OUTFILE $line;
        }
    }
    close INFILE;
    close OUTFILE;
    system("/bin/mv -f ${plugin}_tmp $plugin");
}
system("/bin/rm -rf $bundle/Contents/Plugins");
system("/bin/cp -R $plugin_dir $bundle/Contents/Plugins");

#Framework preparation
print "Do you want to copy Qt frameworks into the bundle automatically (y/n)? ";
my $answer = <STDIN>;
if ($answer !~ /^y/i)
{
    exit 0;
}
system("/bin/mkdir $bundle/Contents/Frameworks");
#otool tells us what frameworks are currently dependencies.
my @otool_info = `/usr/bin/otool -L $bundle/Contents/MacOS/BibleMemorizer`;
foreach $line (@otool_info)
{
    #We only care about Qt frameworks, as the others come with OS X
    if ($line =~ /(\S*Qt[\w\d]+\.framework\S*)/)
    {
        #Full path = name application currently expects
        my $full_path = $1;
        $line =~ /(Qt[\w\d]+\.framework\S*)/;
        #Base path = Qt{whatever}.framework/Versions...
        my $base_path = $1;
        $line =~ /(\S*Qt[\w\d]+\.framework)/;
        my $origin_path = $1;
        #Origin path = /Library/Frameworks/QtCore (e.g.) but might not include
        #/Library/Frameworks
        my $cp_origin_path = $origin_path;
        if ($origin_path !~ /^\//)
        {
            $cp_origin_path = "/Library/Frameworks/" . $origin_path;
        }
        print("Setting up framework $origin_path\n");
        #Copy framework into bundle
        system("/bin/cp -R $cp_origin_path $bundle/Contents/Frameworks");
        #Rename the framework in the bundle
        system("/usr/bin/install_name_tool -id \@executable_path/../Frameworks/$base_path $bundle/Contents/Frameworks/$base_path");
        #Change the executable dependency
        system("/usr/bin/install_name_tool -change $full_path \@executable_path/../Frameworks/$base_path $bundle/Contents/MacOS/BibleMemorizer");
        if (-e "$bundle/Contents/Plugins/libcrosswire.dylib")
        {
            my @test_lines = `/usr/bin/otool -L $bundle/Contents/Plugins/libcrosswire.dylib | grep $origin_path`;
            if (@test_lines > 0)
            {
                system("/usr/bin/install_name_tool -change $full_path \@executable_path/../Frameworks/$base_path $bundle/Contents/Plugins/libcrosswire.dylib");
            }
        }
        #Change the dependency information for frameworks in the bundle
        my @otool_sub_info = `/usr/bin/otool -L $bundle/Contents/Frameworks/$base_path`;
        foreach $line2 (@otool_sub_info)
        {
            if ($line2 =~ /(\S*Qt[\w\d]+\.framework\S*)/)
            {
                my $full_path_2 = $1;
                $line2 =~ /(Qt[\w\d]+\.framework\S*)/;
                my $base_path_2 = $1;
                system("/usr/bin/install_name_tool -change $full_path_2 \@executable_path/../Frameworks/$base_path_2 $bundle/Contents/Frameworks/$base_path");
            }
        }
    }
}
