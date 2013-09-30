#!/usr/bin/env perl
use strict; use warnings;
use CGI;  # Change to "use CGI::Compress::Gzip;" if you have this module and want to compress output
use POSIX qw(strftime);
use CGI::Carp qw(warningsToBrowser fatalsToBrowser);  # uncomment to spit errors at the user instead of the Web server log
use Config::Simple;
use DBI;
                                                                                                                                                             my $cfg = new Config::Simple("/var/lib/adom/etc/config") or die "Unable to open config file $!\n";

my $WATER = 1; my $FIRE = 2; my $AIR = 4; my $EARTH = 8; my $MANA = 16; my $FISTY = 32; my $ANDY = 64;

my %races = ( 'Dr'=>    'drakish', 'Dw'=>    'dwarven', 'De'=>    'dark elven', 'Ge'=>    'gray elven', 'He'=>    'high elven', 'Gn'=>    'gnomish', 'Hm'=>    'human', 'Hr'=>    'hurthling', 'Or'=>    'orcish', 'Tr'=>    'trollish', 'Rt'=>    'ratling', 'Me'=>    'mist elven',);
my %classes = ( 'Ar'=>    'archer', 'As'=>    'assassin', 'Bb'=>    'barbarian', 'Br'=>    'bard', 'Bf'=>    'beastfighter', 'Dr'=>    'druid', 'El'=>    'elementalist', 'Fa'=>    'farmer', 'Fi'=>    'fighter', 'He'=>    'healer', 'Me'=>    'merchant', 'Mi'=>    'mindcrafter', 'Mo'=>    'monk', 'Ne'=>    'necromancer', 'Pa'=>    'paladin', 'Pr'=>    'priest', 'Ra'=>    'ranger', 'Th'=>    'thief', 'We'=>    'weaponsmith', 'Wz'=>    'wizard', 'Ck'=>    'chaos knight', 'Du'=>    'duelist',);

my $cgi = new CGI; # Change to "... new CGI::Compress::Gzip;" if you have this module
print $cgi->header; # HTTP header
print $cgi->start_html(
		-title => "ADOM Server User Stats",
		-script => { -type =>'JAVASCRIPT', -src => "sorttable.js", },
		-style => { -src => '/k.css'},
		);
my $user = undef;
$user = $cgi->param('user');
my $dbh = DBI->connect("dbi:Pg:dbname=" . $cfg->param('DB_NAME') . ";host=" . $cfg->param('SQL_HOST') . ";", $cfg->param('SQL_USER'), $cfg->param('SQL_PASS'));
if (!defined $dbh) { die "Cannot connect to database! $!\n"; }
if (defined($user)) { # display user stats
	my $sth = $dbh->prepare(" SELECT * from stats WHERE username=? ORDER BY date ") or die $dbh->errstr;
	$sth->execute($user) or die $dbh->errstr;

	print "<h2>Stats for $user</h2>\n";
        print "<p>Click any table header to sort.</p>\n";
        print_table_header();

	while (my $row = $sth->fetchrow_hashref()) {
                print_row($row);
	}
        print_table_footer();
	$sth->finish;
}
else { # no username given, display other stats
        my $sth = $dbh->prepare(" SELECT * from stats ORDER BY date DESC LIMIT 10 ") or die $dbh->errstr;
        $sth->execute() or die $dbh->errstr;
        print "<h2>Last 10 games</h2>\n";
        print_table_header();
	while (my $row = $sth->fetchrow_hashref()) {
                print_row($row);
	}
        print_table_footer();
	$sth->finish;
}
$dbh->disconnect;
# ask for an(other) user
print '<p><form name="input" action="stats.pl"><strong>Enter username: </strong><input type="text" name="user"><input type="submit" value="Submit"></form></p>';

print $cgi->end_html;
exit;

sub print_table_header() {
	print "<table class='sortable'><thead><tr>\n";
        my @headers = ("Name", "Score", "Lvl", "R /", "C", "Align", "Game Time", "Turns", "Result", "Bosses", "Date", "Version");
        foreach my $header (@headers) {
                print "<th>$header</th>";
        }                                                                                                                                                            print "</tr></thead><tbody>\n";  
}
sub print_table_footer() {
	print "</tbody></table>\n";
}
sub print_row($) {
        my $row = shift;
        print "<tr>";
        if (!defined($cgi->param('user'))) { $user = $row->{'username'}; }
        print "<td class='left'>$row->{'name'}</td>";
        $row->{'score'} =~ s/(?<=\d)(?=(?:\d\d\d)+(?!\d))/,/g; # commify
        print "<td class='left'>$row->{'score'}</td>";
        print "<td>$row->{'level'}</td>";
        print "<td><span class='tooltip' title='$races{$row->{'race'}}'>$row->{'race'}</span></td>";
        print "<td><span class='tooltip' title='$classes{$row->{'class'}}'>$row->{'class'}</span></td>";
        print "<td>$row->{'align'}</td>";
        print "<td>$row->{'time'}</td>";
        print "<td>$row->{'turns'}</td>";
        print "<td><a href='/adom_users/$user/flgs/$row->{'filename'}'>$row->{'reason'}</a></td>";
        my $bossstr = "";
        if ($row->{'bs'} & $WATER) { $bossstr .= '<span title="Snake from Beyond">s</span>'; }
        if ($row->{'bs'} & $FIRE) { $bossstr .= '<span title="Ancient Chaos Wyrm">W</span>'; }
        if ($row->{'bs'} & $AIR) { $bossstr .= '<span title="Master Summoner">@</span>'; }
        if ($row->{'bs'} & $EARTH) { $bossstr .= '<span title="Ancient Stone Beast">F</span>'; }
        if ($row->{'bs'} & $MANA) { $bossstr .= '<span title="Chaos Archmage">@</span>'; }
        if ($row->{'bs'} & $FISTY) { $bossstr .= '<span title="Fistanarius">&</span>'; }
        if ($row->{'bs'} & $ANDY) { $bossstr .= '<span title="ElDeR cHaOs GoD">@</span>'; }
        print "<td>$bossstr</td>";
        print "<td sorttable_customkey='$row->{'date'}'>" . strftime "%d %b '%y", localtime $row->{'date'} . "</td>";
        print "<td>$row->{'version'}</td>";
        print "</tr>\n";
}
