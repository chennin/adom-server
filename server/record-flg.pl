#!/usr/bin/env perl
use strict;
use warnings;
use POSIX qw(getpwuid);
use File::Basename;
use File::stat;
use Config::Simple;
use DBI;

my $cfg = new Config::Simple("/var/lib/adom/etc/config") or die "Unable to open config file $!\n";
my $filename = $ARGV[0];
if ($#ARGV != 1) { die "Provide a flg name and version"; }
open FLG, "<$filename"or die $!;

my %races = (
    'drakish' => 'Dr', 'dwarven' => 'Dw', 'dark elven' => 'De', 'gray elven' => 'Ge', 'high elven' => 'He', 'gnomish' => 'Gn', 'human' => 'Hm', 'hurthling' => 'Hr', 'orcish' => 'Or', 'trollish' => 'Tr', 'ratling' => 'Rt', 'mist elven' => 'Me',);
my $racereg = join("|", map {quotemeta} keys %races);
my %classes = (
    'archer' => 'Ar', 'assassin' => 'As', 'barbarian' => 'Bb', 'bard' => 'Br', 'beastfighter' => 'Bf', 'druid' => 'Dr', 'elementalist' => 'El', 'farmer' => 'Fa', 'fighter' => 'Fi', 'healer' => 'He', 'merchant' => 'Me', 'mindcrafter' => 'Mi', 'monk' => 'Mo', 'necromancer' => 'Ne', 'paladin' => 'Pa', 'priest' => 'Pr', 'ranger' => 'Ra', 'thief' => 'Th', 'weaponsmith' => 'We', 'wizard' => 'Wz', 'chaos knight' => 'Ck', 'duelist' => 'Du',);
my $classreg = join("|", map {quotemeta} keys %classes);

my $WATER = 1; my $FIRE = 2; my $AIR = 4; my $EARTH = 8; my $MANA = 16; my $FISTY = 32; my $ANDY = 64;

my $user = getpwuid($<); # REAL_USER_ID
my ($name, $align, $score, $level, $race, $class, $reason, $time, $turns, $date, $version) = undef;
$version = $ARGV[1];
if ($version eq "111") { $version = "1.1.1"; }
elsif ($version eq "100") { $version = "1.0.0"; }
my $bs = 0; # boss bitmask
my $catlover = 0;
while (my $line = <FLG>) {
  if ($line =~ /(.+?)\s+St:.*? ([CNL][-=+LNC]?)$/) { # Name and stats line
    $name = $1; $align = $2;
    $name =~ s/^ /_/g;
  }
  elsif ($line =~ /e scored (-?\d+) points(?:\.| and advanced to level (\d{1,2})\.)/) { # level and score
    $level = 1; if (defined $2) { $level = $2; }
    $score = $1; 
  }
  elsif ($line =~ /e survived for/) { # Time, turns
    $line .= <FLG>; # Always seems to be two lines
    if ($line =~ /e survived for (\d+) years?, (\d+) days?,.(\d+).hours?,.(\d+).minutes?.and.(\d+).seconds?.\((\d+).turns?\)\./s) {
      $time = sprintf "%dd %02d:%02d:%02d", $1*360+$2, $3, $4, $5; 
      $turns = $6;
    }
  }
  elsif ((defined($name)) && ($line =~ /^\Q$name\E, the/)) { # name, reason, r/c
    if ($line !~ /\.$/) { $line .= <FLG>; } # not always two lines
    $line =~ /\Q$name\E, the ([^,]+), (.+?)(?:.while.saving|.without.even|\.)/s;
    my $raceclass = lc $1;
    $reason = $2;
    $raceclass =~ s/($racereg)/$races{$1}/go;
    $raceclass =~ s/($classreg)/$classes{$1}/go;
    $raceclass =~ /(\S+) (\S+)/;
    ($race, $class) = ($1, $2);
    $reason =~ s/[\n\r]/ /g;
  }
  elsif ($line =~ /adhered to the principles of the Cat Lord/) { $catlover = 1; }
  elsif ($line =~ /^Version ([0-9.]+)/) {
        $version = $1;
        $line = <FLG>;
        if ($line =~ /Prerelease (\d+)/) { $version .= "p$1" }
  }
  # boss kill tracking
  if ($line =~ /1 Snake from Beyond/) { $bs += $WATER }
  if ($line =~ /1 Ancient Chaos Wyrm/) { $bs += $FIRE }
  if ($line =~ /1 Master Summoner/) { $bs += $AIR }
  if ($line =~ /1 Ancient Stone Beast/) { $bs += $EARTH }
  if ($line =~ /1 Chaos Archmage/) { $bs += $MANA }
  if ($line =~ /1 greater balor/) { $bs += $FISTY }
  if ($line =~ /1 ElDeR cHaOs GoD/) { $bs += $ANDY }
}
close FLG;
$date = stat($filename)->mtime; # modified time will have to do
$filename = basename($filename); # store filename so we don't have to deal with special characyter replacement
#print "$user, $name, $align, $score, $level, $race, $class, $reason, $time, $turns, $catlover, $bs, $date, $filename, $version";

# connect and record in SQL table
my $dbh = DBI->connect("dbi:Pg:dbname=" . $cfg->param('DB_NAME') . ";host=" . $cfg->param('SQL_HOST') . ";", $cfg->param('SQL_USER'), $cfg->param('SQL_PASS'));
if (!defined $dbh) {
   die "Cannot connect to database! $!\n";
}
my $sth = $dbh->prepare("
  INSERT INTO stats (username, name, align, score, level, race, class, reason, time, turns, catlover, bs, date, filename, version) VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?)") or die $dbh->errstr;
$sth->execute($user, $name, $align, $score, $level, $race, $class, $reason, $time, $turns, $catlover, $bs, $date, $filename, $version) or die $dbh->errstr;
$sth->finish;
$dbh->disconnect;
