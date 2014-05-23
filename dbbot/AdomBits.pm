#!/usr/bin/perl
package AdomBits;

use warnings;
use strict;

use Carp;

use Sub::Exporter -setup => { exports => [qw(bytes strd str word short byte sbyte sshort uword bitfield tomatl tomob toitem intf toalign torace toclass tosign toskill toelement parse_num fromsign fromrace fromclass fromspell tospell tospeech set_vers get_vers)] };

sub parse_num {
    my $s = shift;
    $s =~ /^0x/ ? hex($s) : $s;
}

my $adom111;
my $adom120;
my $vers = "120";
sub set_vers($) {
    if ($_[0] =~ /^111|120$/) { $vers = $_[0]; }
}
sub get_vers() { return $vers; }

open ADOM, "<./adom-111" or die "cannot open adom: $!\n";
{
    local $/;
    $adom111 = <ADOM>;
}
close ADOM or die "cannot close adom: $!\n";

open ADOM, "<./adom-120" or die "cannot open adom: $!\n";
{
    local $/;
    $adom120 = <ADOM>;
}
close ADOM or die "cannot close adom: $!\n";

sub bytes
{
    my ($va, $n) = @_;
    if (get_vers() == 111)
    {
        if ($va >= 0x8048000 && $n >= 0 && $n <= (0x2196a0 + 0x8048000 - $va))
        {
            return substr($adom111, $va - 0x8048000, $n);
        }
        if ($va >= 0x082626a0 && $n >= 0 && $n <= (0x3bf1c + 0x082626a0 - $va))
        {
            return substr($adom111, $va - 0x8049000, $n);
        }

    }
    elsif (get_vers() == 120) 
    {
        if ($va >= 0x8048000 && $n >= 0 && $n <= (0x2572a0 + 0x8048000 - $va))
        {
            return substr($adom120, $va - 0x8048000, $n);
        }
        if ($va >= 0x829f2a0 && $n >= 0 && $n <= (0x831bb20 - $va))
        {
            return substr($adom120, $va - 0x8048000, $n);
        }
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
    my $n;
    if (get_vers() == 111) { $n = str(0x8286c40+4*shift()); }
    elsif (get_vers() == 120) { $n = str(0x81E9A80+4*shift()); }

    $n =~ s/ .*//;
    $n;
}

my (%MOB, %ITEM, %ITEMM, %RACE, %CLASS, %SIGN, %SKILL, %SPELL, %SPEECH);
$MOB{111} = 0x8264fc0; $MOB{120} = 0x82A2400;
$RACE{111} = 0x82630e0; $RACE{120} = 0x818D260;
$CLASS{111} = 0x82631a0; $CLASS{120} = 0x818D200;
$SIGN{111} = 0x8299f80; $SIGN{120} = 0x8269A20;
$ITEM{111} = 0x8273b00+0xc; $ITEM{120} = 0x82B09C0+0xc;
$ITEMM{111} = 0x70; $ITEMM{120} = 0x74;
$SKILL{111} = 0x8287ea0; $SKILL{120} = 0x82C59C0;
$SPELL{111} = 0x82875c0; $SPELL{120} = 0x81EF240;
$SPEECH{111} = 0x8272e00; $SPEECH{120} = 0x81DA1E0;

sub tomob { str($MOB{get_vers()}+0x88*shift()) }
sub toitem{ str($ITEM{get_vers()}+$ITEMM{get_vers()}*shift()) }
sub torace{ str($RACE{get_vers()}+4*shift()) }
sub toclass{ str($CLASS{get_vers()}+4*shift()) }
sub tosign{ str($SIGN{get_vers()}+4*shift()) }
sub toalign{ ((qw/C N L/)[shift()]) }
sub toelement{ ((qw/Fire Watr Crsh Rip Elec Acid Cold/)[shift()]) }
sub toskill { str($SKILL{get_vers()}+40*shift()) }
sub tospell { str($SPELL{get_vers()}+32*shift()) }
sub tospeech { str($SPEECH{get_vers()}+8*shift()) }

sub fromspell
{
    my $k = lc(shift());
    if (get_vers() == 111) { ((grep { lc(str(0x82875c0 + 32*$_)) eq $k || lc(str(0x82875c4 + 32*$_)) eq $k } (0 .. 46)), -1)[0] }
    elsif (get_vers() == 120) { ((grep { lc(str(0x81EF240 + 32*$_)) eq $k || lc(str(0x81EF244 + 32*$_)) eq $k } (0 .. 46)), -1)[0] }
}
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
sub intf
{
    if (get_vers() == 111) { $inttab{bitfield(0x8264fc0 + 0x88*shift() + 0x48, 6, 6)} }
    elsif (get_vers() == 120) { $inttab{bitfield(0x82A2400 + 0x88*shift() + 0x48, 6, 6)} }
}

1;
