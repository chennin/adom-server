#! /usr/bin/perl
use warnings;
use strict;

package sorbot::ItemDb;

use AdomBits qw/word str bitfield tomatl get_vers set_vers/;
use List::Util 'max';

my %artnames = (
    0x00c4, "Rolf's Companion",
    0x00c7, "Moon Sickle",
    0x00fc, "Death's Sting",
    0x00fd, "Rolf's Saviour",
    0x0105, "Sun's Messenger",
    0x0106, "Death's Blade",
    0x0119, "Kinslayer",
    0x011a, "Vanquisher",
    0x0121, "Justifier",
    0x012a, "Cat's Claw",
    0x0136, "The Far Slayer",
    0x0138, "True Aim",
    0x0139, "Thunderstroke",
    0x0145, "Black Thumb",
    0x0146, "Bugbiter",
    0x0158, "Trusted One",
    0x015c, "Big Punch",
    0x01a8, "Shezestriakis",
    0x01ab, "Grod",
    0x01ac, "Celestrix",
    0x0224, "Nature's Friend",
    0x0225, "Lust for Glory",
    0x024a, "Skullcrusher",
    0x024b, "Long Sting",
    0x0265, "Needle",
    0x0266, "Sting",
    0x0272, "Whirlwind",
    0x0273, "Nature's Companion",
    0x0274, "Executor",
    0x0278, "Purifier",
    0x0279, "Protector",
    0x027a, "Preserver",
    0x027c, "Hammerhead",
    0x0282, "Ironfist",
    0x028c, "Soaker",
    0x028d, "Serpent's Bite",
    0x028e, "True Strength",
    0x02ab, "Wyrmlance",
);
if (get_vers() == 120) 
{
    $artnames{0x02c1} = "Moloch's Thorns";
    $artnames{0x02c3} = "Foeslammer";
}
my %descriptions = (
);

sub opt_field {
    my ($addr, $bit1, $bitn, @vals) = @_;

    my $val = word($addr);

    for my $i (0 .. $bitn - $bit1) {
        if ($val & (1 << ($bit1 + $i))) {
            return $vals[$i];
        }
    }
    return $vals[$bitn - $bit1 + 1];
}

my @slots = qw/ helmet body shield gauntlet cloak girdle boots necklace bracers
                one-hand two-hand launcher missile tool instrument ring wand
                potion scroll book food gold gem /;
my @wsk = ( '', 'daggers & knives', 'clubs & hammers', 'maces & flails',
    'swords', 'axes', 'whips', 'polearms', 'two-handed weapons', 'staves',
    'slings', 'bows', 'crossbows', 'thrown daggers', 'thrown rocks & clubs',
    'thrown spears', 'boomerangs & scurgari', 'shields');

sub _getentry {
    my $i = shift();
    my $B;
    if (get_vers() == 111) { $B = 0x8273b00 + 0x70*$i; }
    elsif (get_vers() == 120) { $B = 0x82B09C0 + 0x74*$i; }

    my %valhash = (
        index           => $i,
        artname         => $artnames{$i},

        is_artifact     => word($B) & 16384,
        class           => word($B) & 255,
        always_plural   => word($B + 0x4),
        specific_color  => word($B + 0x8),
        singular        => $i == 0x25 ? 'gold piece' : str($B+0x0C),
        plural          => $i == 0x25 ? 'gold pieces' : str($B+0x10),
        unid_singular   => str($B+0x14),
        unid_plural     => str($B+0x18),
        base_dv         => word($B+0x1C),
        base_pv         => word($B+0x20),
        base_melee_hit  => word($B+0x24),
        base_melee_plus => word($B+0x28),
        base_melee_dice => word($B+0x2C),
        base_melee_roll => word($B+0x30),
        base_missl_hit  => word($B+0x34),
        base_missl_plus => word($B+0x38),
        base_missl_dice => word($B+0x3C),
        base_missl_roll => word($B+0x40),
        weight_xxx      => word($B+0x44),
        # Twinge:Value  48
        bonus           => word($B+0x4C),
        dl              => word($B+0x50),
        CHAOS           => bitfield($B+0x54, 0),
        luck_xxx        => bitfield($B+0x54, 3),
        searching_xxx   => bitfield($B+0x54, 4),
        luminous_xxx    => bitfield($B+0x54, 5),
        regen           => bitfield($B+0x54, 7),

        slay_dragon     => bitfield($B+0x54, 10),
        slay_undead     => bitfield($B+0x54, 11),
        slay_unlife     => bitfield($B+0x54, 12),
        slay_humanoid   => bitfield($B+0x54, 13),
        slay_giant      => bitfield($B+0x54, 14),
        slay_demon      => bitfield($B+0x54, 15),
        modifies_stat   => (bitfield($B+0x54, 16)
            ? (bitfield($B+0x54, 30) ? 'Pe'
                : opt_field($B+0x54, 17, 24, qw/St Le Wi Dx To Ch Ap Ma ANY/))
            : undef),

        slay_insect     => bitfield($B+0x54, 26),
        slay_plant      => bitfield($B+0x54, 27),
        autocursing     => bitfield($B+0x54, 28),
        returning       => bitfield($B+0x54, 29),
        #Pe handled above

        rustproof       => bitfield($B+0x58, 0),
        crit_bonus      => bitfield($B+0x58, 1),
        is_arrow_xxx    => bitfield($B+0x58, 2),
        is_euquarrel_xxx=> bitfield($B+0x58, 3),
        is_bow_xxx      => bitfield($B+0x58, 4),
        is_euxbow_xxx   => bitfield($B+0x58, 5),
        is_sling_xxx    => bitfield($B+0x58, 6),
        is_slingable_xxx=> bitfield($B+0x58, 7),
        affects_speed   => bitfield($B+0x58, 8),
        slay_jelly      => bitfield($B+0x58, 9),
        throwable_xxx   => bitfield($B+0x58, 10),
        is_tnyqrrl_xxx  => bitfield($B+0x58, 11),
        affixable       => bitfield($B+0x58, 12),

        int_rfire       => bitfield($B+0x5C, 0),
        int_rpois       => bitfield($B+0x5C, 1),
        int_rcold       => bitfield($B+0x5C, 2),
        int_racid       => bitfield($B+0x5C, 3),
        int_lucky       => bitfield($B+0x5C, 4),
        int_fate        => bitfield($B+0x5C, 5),
        int_cursed      => bitfield($B+0x5C, 6),
        int_rsleep      => bitfield($B+0x5C, 7),
        int_rpetr       => bitfield($B+0x5C, 8),
        int_doomed      => bitfield($B+0x5C, 9),
        int_teleport    => bitfield($B+0x5C, 10),
        int_invisible   => bitfield($B+0x5C, 11),
        int_tcontrol    => bitfield($B+0x5C, 12),
        int_rstun       => bitfield($B+0x5C, 13),
        int_rdeath      => bitfield($B+0x5C, 14),
        int_rpara       => bitfield($B+0x5C, 15),
        int_rshock      => bitfield($B+0x5C, 16),
        int_seeinvis    => bitfield($B+0x5C, 17),
        int_imfire      => bitfield($B+0x5C, 18),
        int_imacid      => bitfield($B+0x5C, 19),
        int_imcold      => bitfield($B+0x5C, 20),
        int_imelec      => bitfield($B+0x5C, 21),
        int_wbre        => bitfield($B+0x5C, 22),
        int_rconf 	=> bitfield($B+0x5C, 24),

        gen_weight      => word($B+0x60),
        material        => word($B+0x64),
        weapon_skill    => word($B+0x68),
        nutrition       => word($B+0x6C),
        #smth            => word($B+0x70),
    );

    if (get_vers() == 120)
    {
        $valhash{terror} = bitfield($B+0x58, 13);
    }

    return \%valhash;
}

my @codes = qw/02 03 10 05 06 07 15 14 12 09 11 04 13 08 16/;
sub colorize {
    my ($cnum, $text) = @_;

    return "\x03" . $codes[$cnum-1] . $text . "\x0F";
}

my @class_sym = qw| [ [ [ [ [ [ [ ' ~ ( ( } / ] { = \ ! ? " % $ * |;

my @matl_color = qw| 7 7 6 6 10 15 15 6 6 14 8 7 7 0 |;

sub _formatentry {
    my ($h) = @_;

    my $color = $h->{specific_color} >= 0 ? $h->{specific_color}
                                          : $matl_color[$h->{material}];

    my $name    = $h->{singular} .
        ($h->{artname} ? sprintf(" \"%s\"", $h->{artname}) : "");
    my $appear  = colorize($color, $class_sym[$h->{class}])
        . ($h->{unid_singular} ? (", " . $h->{unid_singular}) : "");
    my @gen     = $h->{gen_weight} ? sprintf("DL%d(%d)", $h->{dl},
        $h->{gen_weight}) : ();
    my @stats;

    push @stats, sprintf("(Me:%+d,%dd%d%+d)", $h->{base_melee_hit},
        $h->{base_melee_dice}, $h->{base_melee_roll} / $h->{base_melee_dice},
        $h->{base_melee_plus});

    push @stats, sprintf("(Ms:%+d,%dd%d%+d)", $h->{base_missl_hit},
        $h->{base_missl_dice}, $h->{base_missl_roll} / $h->{base_missl_dice},
        $h->{base_missl_plus});

    push @stats, sprintf("[%+d, %+d]", $h->{base_dv}, $h->{base_pv})
        if $h->{base_dv} || $h->{base_pv};

    push @stats, sprintf "{%s%+d}", $h->{modifies_stat}, $h->{bonus}
        if $h->{modifies_stat};

    push @stats, sprintf "(%+d spd)", $h->{bonus}
        if $h->{affects_speed};

    push @stats, "AUTOCURSING" if $h->{autocursing};
    push @stats, "CHAOS" if $h->{CHAOS};
    push @stats, "Light" if $h->{luminous_xxx};
    push @stats, "Luck" if $h->{luck_xxx};
    push @stats, "Search" if $h->{searching_xxx};
    push @stats, "affixable" if $h->{affixable};
    push @stats, "returns" if $h->{returning};
    push @stats, "rustproof" if $h->{rustproof};
    push @stats, "terror" if $h->{terror};
    push @stats, "!Demo" if $h->{slay_demon};
    push @stats, "!Drag" if $h->{slay_dragon};
    push @stats, "!Gian" if $h->{slay_giant};
    push @stats, "!Huma" if $h->{slay_humanoid};
    push @stats, "!Inse" if $h->{slay_insect};
    push @stats, "!Jell" if $h->{slay_jelly};
    push @stats, "!Plan" if $h->{slay_plant};
    push @stats, "!Unde" if $h->{slay_undead};
    push @stats, "!Unli" if $h->{slay_unlife};
    push @stats, "+Crit(" . $h->{bonus} . "%)" if $h->{crit_bonus};
    push @stats, "+Curs" if $h->{int_cursed};
    push @stats, "+Doom" if $h->{int_doomed};
    push @stats, "+Fate" if $h->{int_fate};
    push @stats, "+Invi" if $h->{int_invisible};
    push @stats, "+Luck" if $h->{int_lucky};
    push @stats, "+Rgen" if $h->{regen};
    push @stats, "+SeeI" if $h->{int_seeinvis};
    push @stats, "+TCtr" if $h->{int_tcontrol};
    push @stats, "+Tele" if $h->{int_teleport};
    push @stats, "+WBre" if $h->{int_wbre};
    push @stats, "-Conf" if $h->{int_rconf};
    push @stats, "-Acid" if $h->{int_racid};
    push @stats, "-Cold" if $h->{int_rcold};
    push @stats, "-Deth" if $h->{int_rdeath};
    push @stats, "-Elec" if $h->{int_rshock};
    push @stats, "-Fire" if $h->{int_rfire};
    push @stats, "-Para" if $h->{int_rpara};
    push @stats, "-Petr" if $h->{int_rpetr};
    push @stats, "-Pois" if $h->{int_rpois};
    push @stats, "-Slee" if $h->{int_rsleep};
    push @stats, "-Stun" if $h->{int_rstun};
    push @stats, "=Acid" if $h->{int_imacid};
    push @stats, "=Cold" if $h->{int_imcold};
    push @stats, "=Elec" if $h->{int_imelec};
    push @stats, "=Fire" if $h->{int_imfire};

    push @stats, "nutr=" . $h->{nutrition} if $h->{nutrition};

    push @stats, sprintf "(%ds)", $h->{weight_xxx};

    push @stats, "[" . $slots[$h->{class}];
    $stats[-1] .= ", " . $wsk[$h->{weapon_skill}]
        if $h->{weapon_skill} > 0 && $h->{weapon_skill} != 18;
    $stats[-1] .= "]";

    push @stats, "(" . tomatl($h->{material}) . ")";
    push @stats, "(Art)" if $h->{is_artifact};

    push @stats, @gen;
    push @stats, "#" . $h->{index};

    return "$name ($appear) " . join(" ", @stats);
}

my (%entries111, %entries120);
my (%aliases111, %aliases120);
set_vers(111);
for my $inum (0 .. 0x2B8) {
    my $desc = _getentry($inum);

    my $cname =
        ($desc->{unid_singular} eq 'bloody dagger') ? 'bloody dagger' :
        ($desc->{artname}) ? $desc->{artname} :
        $desc->{singular};

    $entries111{$cname} = _formatentry($desc);

    $aliases111{$cname} = [ $desc->{singular}, $desc->{plural},
        $desc->{unid_singular}, $desc->{unid_plural},
        ($desc->{artname} ? (($desc->{singular} . " \"" . $desc->{artname} . "\""),
                             $desc->{artname}) : ()) ];
}
for my $k (sort keys %aliases111) 
{
    $aliases111{$k} = [ grep { $_ && ! exists $entries111{$_} } @{ $aliases111{$k} } ];
}
set_vers(120);
for my $inum (0 .. 0x2E0) {
    my $desc = _getentry($inum);

    my $cname =
        ($desc->{unid_singular} eq 'bloody dagger') ? 'bloody dagger' :
        ($desc->{artname}) ? $desc->{artname} :
        $desc->{singular};

    $entries120{$cname} = _formatentry($desc);

    $aliases120{$cname} = [ $desc->{singular}, $desc->{plural},
        $desc->{unid_singular}, $desc->{unid_plural},
        ($desc->{artname} ? (($desc->{singular} . " \"" . $desc->{artname} . "\""),
                             $desc->{artname}) : ()) ];
}

for my $k (sort keys %aliases120) {
    $aliases120{$k} = [ grep { $_ && ! exists $entries120{$_} } @{ $aliases120{$k} } ];
    #print "$k:\n";
    #print "   ", join(", ", @{$aliases{$k}}), "\n";
    #print "   ", $entries{$k}, "\n";
}

sub entries 
{ 
    if (get_vers() == 111) { \%entries111; } 
    elsif (get_vers() == 120) { \%entries120; }
}
sub aliases 
{ 
    if (get_vers() == 111) { \%aliases111; }
    elsif (get_vers() == 120) { \%aliases120; }
}

1;
