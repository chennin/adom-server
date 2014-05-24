#! /usr/bin/perl
use warnings;
use strict;

package sorbot::MonsterDb;

use AdomBits qw/word str toalign word tospeech bitfield toitem set_vers get_vers/;
use List::Util 'max';

my (%ceff111, %ceff120);
open CEFF, "sorbot/corpse111.txt" or die "cannot open corpse file: $!\n";
while (my ($a, $b) = ((<CEFF>) =~ /(.*?): (.*)/)) {
    $b =~ y/()/[]/;
    $ceff111{lc $a} = $b;
}
close CEFF;
open CEFF, "sorbot/corpse120.txt"  or die "cannot open corpse file: $!\n";
while (my ($a, $b) = ((<CEFF>) =~ /(.*?): (.*)/)) {
    $b =~ y/()/[]/;
    $ceff120{lc $a} = $b;
}
close CEFF;

my %descriptions = (
    singular                => "Singular name of the monster",
    plural                  => "Plural name - often blank for uniques",
    char                    => "On-screen representation",
    alignment               => "It's possible for a monster to have a per-individual alignment - I think only Ruun does this",
    dv                      => "Many factors - most importantly level - adjust this",
    pv                      => "Remember that being a demon or undead devalues this",
    hitdice                 => "Monster HP is (Dice)dX + Add; X is 3..9 by your XL, min 7 if unique",
    hpadd                   => "Monster HP is (Dice)dX + Add; X is 3..9 by your XL, min 7 if unique",
    attacks                 => "Level increases this, rage doubles",
    tohit_xxx               => "Factored into melee, archery, and breath weapon miss calculations; details unknown",
    damage                  => "Just a straight roll, although criticals and armor punching increase",
    size                    => "Corpse weight factor, also influences a few other minor things like corruption tolerance",
    dl                      => "Cannot be generated if (XL + DL) / 2 is less than this, except on an OOD roll",
    prob                    => "Only meaningful when considered alongside others; Thrundarr only assigns >=25; inversely proportional to uber rate",
    color                   => "CGA IRGB format; exception: 17 is karmic",
    dragon                  => "dragon slayers work",
    undead                  => "undead slayers work, immune to poison and mindcraft",
    unlife                  => "unlife slayers work, immune to poison and mindcraft",
    humanoid                => "humanoid slayers work, can be pickpocketed",
    vortex_and_elemental_xxx => "",
    animal                  => "peaceful to druids (unless chaos rat/lizard)",
    cat                     => "affects kitty karma",
    giant                   => "giant slayers work",
    demon                   => "demon slayers work",
    spider_xxx              => "",
    insect                  => "insect slayers work",
    plant                   => "plant slayers work",
    attack_range            => "also gives bonuses to detection",
    breath_dam_xxx          => "interpretation unknown",
    goes_THUMB              => "",
    unknown_44_29           => "<Twinge> Ah, clearly the uh... 'Bad Ass' monsterClassFlag.",
    aquatic                 => "move bonus in water, not recolored in WDL",
    jelly                   => "jelly slayers work",
    tracking                => "chance of following your steps on a turn; perfect tracking appears related to diagonal movement",
    intel                   => "many effects; 0 is mindcraft immune",
    smashes_doors           => "",
    stays_put               => "set on a lot of stationary mobs",
    no_fear                 => "cannot panic, immune to Scare Monster, unholy aura",
    unknown_48_17           => "",
    unknown_48_18           => "wall beasts",
    basic_speed             => "+- 10%",
    gender                  => "* is 20% F 80% M; Shyssyryxius alone among always-F is described as F",
    sp_death_ray            => "incredible magical energies / energy bolt, was extinguished",
    sp_en_stream            => "you are hit by a painful stream of energy",
    sp_confusion            => "",
    sp_glow_balls           => "",
    sp_cone_cold            => "",
    sp_shock_bolt           => "",
    sp_curse                => "PC OR items; can doom",
    sp_stat_drain           => "",
    sp_tp_other             => "",
    sp_darkness             => "",
    sp_full_heal            => "",
    sp_heal                 => "",
    sp_invis                => "",
    spell_range             => "altered by level and monthday",
    spell_power             => "influences damage mostly",
    corrupt_touch           => "melee is 1d(Special1), also corrups on mindcraft, vulnerable to pocc",
    digs                    => "",
    poison_melee            => "",
    suicide_melee           => "like a bee",
    paralyze_touch          => "like a ghul",
    drain_str               => "like a shadow",
    spins_webs              => "",
    doppelganger            => "confusion unless Raven",
    breath_wpn              => "",
    aging_touch             => "ghosts",
    slowing_touch           => "",
    lethal_wail             => "banshee",
    sleep_song              => "harpies, etc",
    sleep_touch             => "homunculi",
    teleportitis            => "like a pixie",
    steals_gold             => "",
    summoner                => "not a spell!; 1/Special2 chance per turn in melee",
    breeder1                => "breeders are in two groups, I'm not sure why",
    regenerate              => "Special1 per turn",
    drain_to                => "like a wight",
    eat_objects             => "like a gelatinous cube",
    sicken_touch            => "like a corpse fiend",
    missile_wpn             => "",
    disarms                 => "",
    breeder2_xxx            => "",
    ignores_pv              => "like a claw bug",
    karmic_breath           => "breathes basic4",
    eat_iron                => "rust monster",
    annihilate              => "melee item destruction",
    confuse_touch           => "jackal demon, etc",
    adjust_power            => "Mob DL + Mob XL >= Your XL",
    adjust_quiet            => "Like previous, but no message and no discovery",
    death_ray_res           => "",
    see_invisible           => "",
    unique                  => "cannot be petrified, necroraised, wished for, swapped places with",
    ignore_walls            => "",
    ignore_water            => "will cross, but slowed down by it",
    destroy_webs            => "always destroys, cannot be caught in; hoeuland's fire and constructs",
    ignore_webs             => "but without destroying them; ignore walls implies this",
    passive_acid            => "degrades weapons and gloves",
    mimic                   => "",
    psv_paralyze            => "",
    immune_fire             => "",
    immune_elec             => "",
    immune_cold             => "",
    immune_acid             => "",
    im_petrify              => "houeland's unlife and more",
    vuln_fire               => "",
    vuln_cold               => "",
    vuln_acid               => "",
    vuln_elec               => "",
    karmic_being            => "bad luck to hit",
    align_drop              => "proportional to XP?",
    ignore_traps            => "hoeuland's flyers minotaurs more",
    drain_pp                => "",
    darksight               => "",
    immune_pois             => "implied by undead, unlife; houeland's will o wisp special ability",
    special1                => "affects the magnitude of some flags' effects - corrupting touch amount, regen armount are known",
    special2                => "affects the magnitude of some flags' effects - summoner frequency is known",
    sr                      => "percent chance of shrugging bolts; high level chars get a smallish discount (no more than 25-35)",
    speech                  => "verb for ball spells, etc",
    regen_turns             => "turns between regaining 1 (or 2?) HP; stacks with regenerator",
    corpse_chance           => "pre-Food Pres, pre-of hunting",
    corpse_effect           => "not extracted",
    male_descr              => "",
    female_descr            => ""
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

sub _getentry {
    my $B;
    if (get_vers() == 111) {  $B = 0x8264fc0 + 0x88*shift(); }
    elsif (get_vers() == 120) { $B = 0x82A2400 + 0x88*shift(); }

    my %valhash = (
        singular      => str($B+0x00),
        plural        => str($B+0x04),
        char          => chr(word($B+0x08)),
        alignment     => toalign(word($B+0x0C)),
        dv            => word($B+0x10),
        pv            => word($B+0x14),
        hitdice       => word($B+0x18),
        hpadd         => word($B+0x1C),
        attacks       => word($B+0x20),
        tohit_xxx     => word($B+0x24),
        damage        => str($B+0x28),
        size          => ((word($B+0x2C) * 60) || 30),
        dl            => word($B+0x38),
        color         => word($B+0x40),
        dragon        => bitfield($B+0x44, 0),
        undead        => bitfield($B+0x44, 1),
        unlife        => bitfield($B+0x44, 2),
        humanoid      => bitfield($B+0x44, 3),
        vortex_and_elemental_xxx => bitfield($B+0x44, 4),
        animal        => bitfield($B+0x44, 5),
        cat           => bitfield($B+0x44, 6),
        giant         => bitfield($B+0x44, 7),
        demon         => bitfield($B+0x44, 8),
        spider_xxx    => bitfield($B+0x44, 9),
        insect        => bitfield($B+0x44, 10),
        plant         => bitfield($B+0x44, 11),
        attack_range  => opt_field($B+0x44, 13, 21, 1,2,3,4,5,6,8,10,12, 0),
        breath_dam_xxx=> opt_field($B+0x44, 22, 27, 1,2,3,4,6,8, 0),
        goes_THUMB    => bitfield($B+0x44, 28),
        unknown_44_29 => bitfield($B+0x44, 29),
        aquatic       => bitfield($B+0x44, 30),
        jelly         => bitfield($B+0x44, 31),
        tracking      => opt_field($B+0x48, 0, 5, 20,45,70,90,98,100, 0),
        intel         => opt_field($B+0x48, 6, 11, 0,25,55,75,90,100),
        smashes_doors => bitfield($B+0x48, 13),
        stays_put     => bitfield($B+0x48, 15),
        no_fear       => bitfield($B+0x48, 16),
        unknown_48_17 => bitfield($B+0x48, 17),
        unknown_48_18 => bitfield($B+0x48, 18),
        basic_speed   => word($B+0x4c),
        gender        => ({0=>"'M'", 1=> "'F'", 2=> "'-'", 4=>"'*'", 5=>"'F'" })->{word($B+0x50)},
        sp_death_ray  => bitfield($B+0x54, 25),
        sp_en_stream  => bitfield($B+0x54, 24),
        sp_confusion  => bitfield($B+0x54, 23),
        sp_glow_balls => bitfield($B+0x54, 22),
        sp_cone_cold  => bitfield($B+0x54, 21),
        sp_shock_bolt => bitfield($B+0x54, 20),
        sp_curse      => bitfield($B+0x54, 12),
        sp_stat_drain => bitfield($B+0x54, 11),
        sp_tp_other   => bitfield($B+0x54, 10),
        sp_darkness   => bitfield($B+0x54, 3),
        sp_full_heal  => bitfield($B+0x54, 2),
        sp_heal       => bitfield($B+0x54, 1),
        sp_invis      => bitfield($B+0x54, 0),
        spell_range   => (word($B+0x58) >> 6),
        spell_power   => (word($B+0x58) & 63),
        corrupt_touch => bitfield($B+0x5c, 0),
        digs          => bitfield($B+0x5c, 1),
        poison_melee  => bitfield($B+0x5c, 2),
        suicide_melee => bitfield($B+0x5c, 3),
        paralyze_touch=> bitfield($B+0x5c, 4),
        drain_str     => bitfield($B+0x5c, 5),
        spins_webs    => bitfield($B+0x5c, 6),
        doppelganger  => bitfield($B+0x5c, 7),
        breath_wpn    => bitfield($B+0x5c, 8),
        aging_touch   => bitfield($B+0x5c, 9),
        slowing_touch => bitfield($B+0x5c, 10),
        lethal_wail   => bitfield($B+0x5c, 11),
        sleep_song    => bitfield($B+0x5c, 13),
        sleep_touch   => bitfield($B+0x5c, 14),
        teleportitis  => bitfield($B+0x5c, 15),
        steals_gold   => bitfield($B+0x5c, 16),
        summoner      => bitfield($B+0x5c, 17),
        breeder1      => bitfield($B+0x5c, 18),
        regenerate    => bitfield($B+0x5c, 19),
        drain_to      => bitfield($B+0x5c, 20),
        eat_objects   => bitfield($B+0x5c, 21),
        sicken_touch  => bitfield($B+0x5c, 22),
        missile_wpn   => bitfield($B+0x5c, 23),
        disarms       => bitfield($B+0x5c, 24),
        breeder2_xxx  => bitfield($B+0x5c, 25),
        ignores_pv    => bitfield($B+0x5c, 26),
        karmic_breath => bitfield($B+0x5c, 27),
        eat_iron      => bitfield($B+0x5c, 28),
        annihilate    => bitfield($B+0x5c, 29),
        confuse_touch => bitfield($B+0x5c, 30),
        adjust_power  => bitfield($B+0x60, 0),
        adjust_quiet  => bitfield($B+0x60, 1),
        death_ray_res => bitfield($B+0x64, 0),
        see_invisible => bitfield($B+0x64, 1),
        unique        => bitfield($B+0x64, 2),
        ignore_walls  => bitfield($B+0x64, 3),
        ignore_water  => bitfield($B+0x64, 4),
        destroy_webs  => bitfield($B+0x64, 5),
        ignore_webs   => bitfield($B+0x64, 6),
        passive_acid  => bitfield($B+0x64, 7),
        mimic         => bitfield($B+0x64, 8),
        psv_paralyze  => bitfield($B+0x64, 9),
        immune_fire   => bitfield($B+0x64, 10),
        immune_elec   => bitfield($B+0x64, 11),
        immune_cold   => bitfield($B+0x64, 12),
        immune_acid   => bitfield($B+0x64, 13),
        im_petrify    => bitfield($B+0x64, 14),
        vuln_fire     => bitfield($B+0x64, 15),
        vuln_cold     => bitfield($B+0x64, 16),
        vuln_acid     => bitfield($B+0x64, 17),
        vuln_elec     => bitfield($B+0x64, 18),
        karmic_being  => bitfield($B+0x64, 19),
        align_drop    => bitfield($B+0x64, 20),
        ignore_traps  => bitfield($B+0x64, 21),
        drain_pp      => bitfield($B+0x64, 22),
        darksight     => bitfield($B+0x64, 23),
        immune_pois   => bitfield($B+0x64, 24),
        special1      => word($B+0x68),
        special2      => word($B+0x6c),
        sr            => word($B+0x70),
        speech        => tospeech(word($B+0x74)),
        regen_turns   => word($B+0x78),
        corpse_chance => word($B+0x7c),
        male_descr    => str($B+0x80),
        female_descr  => str($B+0x84)
    );

    if (get_vers() == 111)
    {
        $valhash{prob} = word($B+0x3C); 
        $valhash{corpse_effect} = ($ceff111{lc(str($B))}),
    }
    elsif (get_vers() == 120)
    {
        $valhash{prob} = (word($B+0x3C) / 10);
        $valhash{corpse_effect} = ($ceff120{lc(str($B))}),
    }

    return \%valhash;
}

my @codes = qw/02 03 10 05 06 07 15 14 12 09 11 04 13 08 16/;
sub colorize {
    my ($cnum, $text) = @_;

    if ($cnum == 17) { return "color-changing $text"; }

    return "\x03" . $codes[$cnum-1] . $text . "\x0F";
}


sub _formatentry {
    my ($h) = @_;

    my $fixed = '';
    my @flags;
    my @spell;
    my @immune;
    my @vuln;
    my @spmelee;

    push @flags, sprintf("dl%d%s", $h->{dl}, $h->{prob} ? ("(".$h->{prob}.")") : "") if $h->{prob} || ($h->{dl} != 1);
    push @vuln, "dragon slaying" if $h->{dragon};
    push @vuln, "undead slaying" if $h->{undead};
    push @vuln, "construct slaying" if $h->{unlife};
    push @vuln, "humanoid slaying" if $h->{humanoid};
    push @flags, "vortex or elemental?" if $h->{vortex_and_elemental_xxx};
    push @flags, "animal" if $h->{animal};
    push @flags, "cat" if $h->{cat};
    push @vuln, "giant slaying" if $h->{giant};
    push @vuln, "demon slaying" if $h->{demon};
    push @flags, "spider" if $h->{spider};
    push @vuln, "Bugbiter" if $h->{insect};
    push @vuln, "plant slaying" if $h->{plant};
    # attack range, breath dam only displayed if relevant flag
    push @flags, "thumb" if $h->{goes_THUMB};
    push @flags, "? 44 29" if $h->{unknown_44_29};
    push @flags, "aquatic" if $h->{aquatic};
    push @vuln, "jelly slaying" if $h->{jelly};
    push @flags, sprintf("tracking(%d%%)", $h->{tracking}) if $h->{tracking};
    push @flags, sprintf("intel(%d%%)", $h->{intel}) if $h->{intel};
    push @immune, "mindcraft" if !$h->{intel} || $h->{undead} || $h->{unlife};
    push @flags, "diagonal walk" if $h->{tracking} == 100;
    push @immune, "locked doors" if $h->{smashes_doors};
    push @flags, "stays put" if $h->{stays_put};
    push @immune, "fear" if $h->{no_fear};
    push @flags, "? 48 17" if $h->{unknown_48_17};
    push @flags, "? 48 18" if $h->{unknown_48_18};
    push @flags, "male" if $h->{gender} eq 'M';
    push @flags, "female" if $h->{gender} eq 'F';
    push @flags, "neuter" if $h->{gender} eq '-';
    push @spell, "death ray" if $h->{sp_death_ray};
    push @spell, "energy" if $h->{sp_en_stream};
    push @spell, "confuse" if $h->{sp_confusion};
    push @spell, "fire" if $h->{sp_glow_balls};
    push @spell, "cold" if $h->{sp_cone_cold};
    push @spell, "shock" if $h->{sp_shock_bolt};
    push @spell, "curse" if $h->{sp_curse};
    push @spell, "stat drain" if $h->{sp_stat_drain};
    push @spell, "teleport" if $h->{sp_tp_other};
    push @spell, "darken" if $h->{sp_darkness};
    push @spell, "full heal" if $h->{sp_full_heal};
    push @spell, "heal" if $h->{sp_heal};
    push @spell, "invis" if $h->{sp_invis};

    if (@spell) {
        push @flags, sprintf "SPELLS(%s; range %d power %d)", join(", ", sort @spell), $h->{spell_range}, $h->{spell_power};
    }

    push @spmelee, sprintf("corrupt 1d%d", $h->{special1}) if $h->{corrupt_touch};
    push @flags, "digs" if $h->{digs};
    push @spmelee, sprintf("poison %dfr %ddam", $h->{special1}, $h->{special2}) if $h->{poison_melee};
    push @spmelee, "suicidally" if $h->{suicide_melee};
    push @spmelee, sprintf("paralyze 1d%d", $h->{special2}) if $h->{paralyze_touch};
    push @spmelee, "drain st" if $h->{drain_str};
    push @flags, "spins webs" if $h->{spins_webs};
    push @spmelee, "doppelganger confusion" if $h->{doppelganger};
    push @flags, sprintf("breathes %s %d r %dpow 1:%d", ($h->{karmic_breath} ? "everything" : "something"), $h->{attack_range}, $h->{breath_dam_xxx}, $h->{special1}) if $h->{breath_wpn};
    push @spmelee, "ages" if $h->{aging_touch}; # XXX investigate amount
    push @spmelee, sprintf("slows 1d%d", $h->{special1}) if $h->{slowing_touch};
    push @flags, "lethal wail" if $h->{lethal_wail};
    push @flags, "lullaby" if $h->{sleep_song};
    push @flags, "teleports" if $h->{teleportitis};
    push @spmelee, "steal gold" if $h->{steals_gold}; # XXX amount
    push @flags, "summons 1:" . $h->{special2} if $h->{summoner}; # XXX what?
    push @flags, "breeder" if $h->{breeder1} || $h->{breeder2_xxx}; # XXX ???
    push @flags, "regenerate(" . $h->{special1} . "/turn)" if $h->{regenerate};
    push @spmelee, "drain to" if $h->{drain_to};
    push @flags, "eats objects" if $h->{eat_objects};
    push @spmelee, "sickness" if $h->{sicken_touch};
    push @flags, "archer(".toitem($h->{special2})."; ".$h->{attack_range}." range; 1:".$h->{special1}.")" if $h->{missile_wpn};
    push @spmelee, "disarmer" if $h->{disarms};
    push @spmelee, "ignore PV" if $h->{ignores_pv};
    push @spmelee, "rusts and destroys worn iron" if $h->{eat_iron};
    push @spmelee, "destroys worn items" if $h->{annihilate};
    push @spmelee, "confuses" if $h->{confuse_touch};
    push @flags, "adjusts in power" if $h->{adjust_power};
    push @flags, "adjusts in power (silently)" if $h->{adjust_quiet};
    push @immune, "death rays" if $h->{death_ray_res};
    push @flags, "sees " . join(" and ", ($h->{darksight} ? ("in darkness") : ()), ($h->{see_invisible} ? ("through invisibility") : ())) if $h->{darksight} || $h->{see_invisible};
    push @flags, "unique" if $h->{unique};
    push @flags, "passes stone and webs" if $h->{ignore_walls};
    push @immune, "water" if $h->{ignore_water};
    push @flags, "destroys webs instantly" if $h->{destroy_webs};
    push @immune, "webs" if $h->{ignore_webs};
    push @flags, "acidic counterattack" if $h->{passive_acid};
    push @flags, "mimics items" if $h->{mimic};
    push @flags, "paralyzing counterattack" if $h->{psv_paralyze};
    push @immune, "fire" if $h->{immune_fire};
    push @immune, "elec" if $h->{immune_elec};
    push @immune, "cold" if $h->{immune_cold};
    push @immune, "acid" if $h->{immune_acid};
    push @immune, "petrify" if $h->{im_petrify} || $h->{unique};
    push @vuln, "fire" if $h->{vuln_fire};
    push @vuln, "cold" if $h->{vuln_cold};
    push @vuln, "acid" if $h->{vuln_acid};
    push @vuln, "elec" if $h->{vuln_elec};
    push @flags, "karmic counterattack" if $h->{karmic_being};
    push @flags, "chaotic to kill" if $h->{align_drop};
    push @immune, "traps" if $h->{ignore_traps};
    push @flags, "drains pp" if $h->{drain_pp};
    push @immune, "poison" if $h->{immune_pois} || $h->{unlife} || $h->{undead};
    push @flags, "sr(" . $h->{sr} . "%)" if $h->{sr};
    push @flags, "corpse(" . $h->{corpse_chance} . ($h->{corpse_effect} ?
        ("%; " . $h->{corpse_effect}) : "%") .
        ")" if $h->{corpse_chance};

    push @flags, "VULN(" . join(", ", sort @vuln) . ")" if (@vuln);
    push @flags, "IMMUNE(" . join(", ", sort @immune) . ")" if (@immune);

    return sprintf "%s (%s): %s, %dspd, %d-%d/%d hp, %ddv, %dpv, %dx %s [%+d]%s, %ds%s",
        ucfirst($h->{singular}), colorize($h->{color}, $h->{char}), $h->{alignment},
        $h->{basic_speed},
        max(1, $h->{hpadd} + $h->{hitdice}),
        max(1, $h->{hpadd} + ($h->{unique} ? 7 : 3) * $h->{hitdice}),
        max(1, $h->{hpadd} + 9 * $h->{hitdice}),
        $h->{dv}, $h->{pv}, $h->{attacks}, $h->{damage}, $h->{tohit_xxx},
        (@spmelee ? (" (" . join(", ", sort @spmelee) . ")") : ""),
        $h->{size}, join("", map { ", $_" } sort @flags);
}

my (%entries111, %entries120);
set_vers(111);
for my $mob (0 .. 0x1A1) {
    my $desc = _getentry($mob);
    $entries111{$desc->{singular}} = _formatentry($desc);
}
set_vers(120);
for my $mob (0 .. 0x1AD) {
    my $desc = _getentry($mob);
    $entries120{$desc->{singular}} = _formatentry($desc);
}

sub entries
{
    if (get_vers() == 111) { \%entries111; }
    elsif (get_vers() == 120) { \%entries120; }
}
sub aliases { +{} }

1;
