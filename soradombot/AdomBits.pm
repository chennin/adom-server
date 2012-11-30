#! /usr/bin/perl
package AdomBits;

use warnings;
use strict;

use Carp;

use Sub::Exporter -setup => { exports => [qw(bytes strd str word short byte sbyte sshort uword bitfield tomatl tomob toitem intf toalign torace toclass tosign toskill toelement parse_num fromsign fromrace fromclass fromspell tospell tospeech)] };

sub parse_num {
    my $s = shift;
    $s =~ /^0x/ ? hex($s) : $s;
}

my $adom;

open ADOM, "<./adom" or die "cannot open adom: $!\n";
{
    local $/;
    $adom = <ADOM>;
}
close ADOM or die "cannot close adom: $!\n";

sub bytes
{
    my ($va, $n) = @_;

    if ($va >= 0x8048000 && $n >= 0 && $n <= (0x2196a0 + 0x8048000 - $va))
    {
        return substr($adom, $va - 0x8048000, $n);
    }

    if ($va >= 0x082626a0 && $n >= 0 && $n <= (0x3bf1c + 0x082626a0 - $va))
    {
        return substr($adom, $va - 0x8049000, $n);
    }

    confess "file access out of range $va $n";
}

sub strd
{
    my $o = shift();
    my $s = '';

    while(1)
    {
        my $c = bytes($o, 1);
        return $s if $c eq "\0";
        $o++;
        $s .= $c;
    }
}

sub uword  { unpack "L<", bytes(shift(), 4); }
sub short  { unpack "S<", bytes(shift(), 2); }
sub byte   { unpack "C",  bytes(shift(), 1); }

sub word   { unpack "l<", bytes(shift(), 4); }
sub sshort { unpack "s<", bytes(shift(), 2); }
sub sbyte  { unpack "c",  bytes(shift(), 1); }

sub str { strd(word(shift())); }

sub tomatl {
    my $n = str(0x8286c40+4*shift());
    $n =~ s/ .*//;
    $n;
}

sub tomob { str(0x8264fc0+0x88*shift()) }
sub toitem{ str(0x8273b00+0xc+0x70*shift()) }
sub torace{ str(0x82630e0+4*shift()) }
sub toclass{ str(0x82631a0+4*shift()) }
sub tosign{ str(0x8299f80+4*shift()) }
sub toalign{ ((qw/C N L/)[shift()]) }
sub toelement{ ((qw/Fire Watr Crsh Rip Elec Acid Cold/)[shift()]) }
sub toskill { str(0x8287ea0+40*shift()) }
sub tospell { str(0x82875c0+32*shift()) }
sub tospeech { str(0x8272e00+8*shift()) }

sub fromspell { my $k = lc(shift()); ((grep { lc(str(0x82875c0 + 32*$_)) eq $k || lc(str(0x82875c4 + 32*$_)) eq $k } (0 .. 46)), -1)[0] }
sub fromsign { my $k = lc(shift()); ((grep { lc(tosign($_)) eq $k } (0 .. 11)), -1)[0] }
sub fromrace { my $k = lc(shift()); ((grep { lc(torace($_)) eq $k } (0 .. 9)), -1)[0] }
sub fromclass { my $k = lc(shift()); ((grep { lc(toclass($_)) eq $k } (0 .. 19)), -1)[0] }

sub bitfield
{
    my $of = shift;
    my $bitof = shift;
    my $bitsz = shift || 1;

    (word($of) >> $bitof) & ((1 << $bitsz) - 1);
}

my %inttab = (1, 0, 2, 25, 4, 55, 8, 75, 16, 90, 32, 100);
sub intf { $inttab{bitfield(0x8264fc0 + 0x88*shift() + 0x48, 6, 6)} }

1;
