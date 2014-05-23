#! /usr/bin/perl
use warnings;
use strict;

package sorbot::Database;

use List::Util 'max', 'min';
use String::Approx 'adist';
use Text::LevenshteinXS 'distance';
use AdomBits qw/set_vers get_vers/;

sub mydist {
    my ($long, $longer) = @_;
    my $sa = abs adist(uc $long, uc $longer);
    my $tl = distance(uc $long, uc $longer);

    ($sa == $tl) ? $sa : $sa+1;
}

sub lookup {
    my ($db, $q, $vers) = @_;
    set_vers($vers);
    my @cand;

    my $f = $db;
    $f =~ s|::|/|g;
    require "$f.pm";

    if ($q =~ m|^/(.*)/$|) {
        my $re = $1;
        @cand = sort grep { $db->entries->{$_} =~ /$re/i } keys %{$db->entries};
    } else {
        my $mind = 3;
        @cand = ();

        for my $cn (sort keys %{$db->entries}) {
            for my $k ($cn, @{ $db->aliases->{$cn} // [] }) {
                my $d = mydist $q, $k;
                next unless $d <= $mind;
                @cand = () if $d < $mind;
                $mind = $d;
                push @cand, $cn if !@cand || $cand[-1] ne $cn;
            }
        }
    }

    if (@cand == 0) {
        return "No match.";
    } elsif (@cand == 1) {
        return "\x02[$vers]\x02 " . $db->entries->{$cand[0]};
    } else {
        my $r = (join ", ", @cand);
        return length($r) < 900 ? $r : "You are not allowed to ask that.";
    }
}

unless ($INC{'Database.pm'}) {
    my $db = shift @ARGV;

    for (@ARGV) { print lookup($db, $_), "\n"; }
}

1;
