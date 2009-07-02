#!/usr/bin/perl

use strict;
use warnings;

use File::Basename;
use File::Spec::Functions qw(rel2abs);

my $in_git = 0;
my $scriptpath = rel2abs(dirname($0));
my $outfile = $ARGV[0];

my $releasever;

if (open RELEASE, "<", "$scriptpath/release_ver") {
	$releasever = <RELEASE>;
	close RELEASE;
}

print "Is this project under Git? ";
if (-d "$scriptpath/../.git" ) {
	print "Yes\n";
	print "Is Git installed? ";
	if ( ! system("which git &> /dev/null") ) {
		print "Yes\n";
		$in_git = 1;
	} else {
		print "No\n";
		$in_git = 0;
	}
} else {
	print "No\n";
	$in_git = 0;
}

my $verstring = "";

if ($in_git == 0) {
	$verstring = $releasever;
} else {
	$verstring = `git describe --tags --long 2> /dev/null || git describe --tags`;
}

chomp($verstring);

my $pattern = "([0-9]).([0-9]).([0-9])(?:(?:-([a-zA-Z]+[0-9]+))?(?:-([0-9]+)-g[a-fA-F0-9]+)?)?";

if ($verstring =~ $pattern) {
} else {
	die "Version string '$verstring' is malformed...\n";
}

my $major = $1;
my $minor = $2;
my $revis = $3;
my $build = $5;
my $pre   = $4;

if ( !$build ) {
	$build = "0";
}

if ( $pre ) {
	# We have a prerelease version.
	$pre = "-$pre";
} else {
	$pre = "";
}

unlink("$outfile.tmp");

my $prefix = "CC_LIB";
my $tag    = "cc";

open OUT, ">", "$outfile.tmp" or die $!;
print OUT <<__eof__;
#ifndef __included_${tag}_build_number_h
#define __included_${tag}_build_number_h

#define ${prefix}_VERSION_MAJOR ${major}
#define ${prefix}_VERSION_MINOR ${minor}
#define ${prefix}_VERSION_REVISION ${revis}
#define ${prefix}_VERSION_BUILD ${build}
#define ${prefix}_VERSION \"${major}.${minor}.${revis}${pre}\"
#define ${prefix}_VERSION_STRING "${verstring}"

#define ${prefix}_RESOURCE_VERSION ${major},${minor},${revis},${build}
#define ${prefix}_RESOURCE_VERSION_STRING \"${major}, ${minor}, ${revis}, ${build}\"

#endif

__eof__
close OUT or die $!;

use Digest::MD5;

my $ctx = Digest::MD5->new;

my $md5old = ""; my $md5new = "";

if (-e $outfile) {
	open OUT, "$outfile" or die $!;
	$ctx->addfile(*OUT);
	$md5old = $ctx->hexdigest;
	close OUT
}

open OUT, "$outfile.tmp" or die $!;
$ctx->addfile(*OUT);
$md5new = $ctx->hexdigest;
close OUT;

use File::Copy;

if ($md5old ne $md5new) {
	if (-e $outfile) {
		unlink($outfile) or die $!;
	}
	move "$outfile.tmp", $outfile or die $!;
	print "$outfile updated.\n";
} else {
	unlink ("$outfile.tmp");
	print "$outfile is already up to date.\n";
}
