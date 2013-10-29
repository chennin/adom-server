#!/usr/bin/env perl
use strict; use warnings;
use CGI;
use POSIX qw(strftime);
use CGI::Carp qw(warningsToBrowser fatalsToBrowser);  # uncomment to spit errors at the user instead of the Web server log
use Config::Simple;
use HTML::Entities;
use DBI;
sub print_table_header();
sub print_table_footer();
sub print_row($);

my $cfg = new Config::Simple("/var/lib/adom/etc/config") or die "Unable to open config file $!\n";

my $WATER = 1; my $FIRE = 2; my $AIR = 4; my $EARTH = 8; my $MANA = 16; my $FISTY = 32; my $ANDY = 64;

my %races = ( 'Dr'=>    'drakish', 'Dw'=>    'dwarven', 'De'=>    'dark elven', 'Ge'=>    'gray elven', 'He'=>    'high elven', 'Gn'=>    'gnomish', 'Hm'=>    'human', 'Hr'=>    'hurthling', 'Or'=>    'orcish', 'Tr'=>    'trollish', 'Rt'=>    'ratling', 'Me'=>    'mist elven',);
my %classes = ( 'Ar'=>    'archer', 'As'=>    'assassin', 'Bb'=>    'barbarian', 'Br'=>    'bard', 'Bf'=>    'beastfighter', 'Dr'=>    'druid', 'El'=>    'elementalist', 'Fa'=>    'farmer', 'Fi'=>    'fighter', 'He'=>    'healer', 'Me'=>    'merchant', 'Mi'=>    'mindcrafter', 'Mo'=>    'monk', 'Ne'=>    'necromancer', 'Pa'=>    'paladin', 'Pr'=>    'priest', 'Ra'=>    'ranger', 'Th'=>    'thief', 'We'=>    'weaponsmith', 'Wz'=>    'wizard', 'Ck'=>    'chaos knight', 'Du'=>    'duelist',);

my $cgi = new CGI;
print $cgi->header; # HTTP header
#use CGI::WebGzip;
print $cgi->start_html(
		-title => "ADOM Server User Stats",
		-script => { -type =>'JAVASCRIPT', -src => "sorttable.js", },
		-style => { -src => '/k.css'},
		);
my $user = undef;
$user = $cgi->param('user');
my $MAX_DATE = 2147483647; # change me & schema before 2038
my $skipts = $MAX_DATE;
if (defined($cgi->param('skipts')) && ($cgi->param('skipts') =~ /^[0-9]+$/)) {
  $skipts = $cgi->param('skipts');
}
my $dbh = DBI->connect("dbi:Pg:dbname=" . $cfg->param('DB_NAME') . ";host=" . $cfg->param('SQL_HOST') . ";", $cfg->param('SQL_USER'), $cfg->param('SQL_PASS'));
if (!defined $dbh) { die "Cannot connect to database! $!\n"; }
if (defined($user)) { # display user stats
        $user = encode_entities($user);
        my $sth = $dbh->prepare(" SELECT * from stats WHERE username=? AND date <=? ORDER BY date DESC LIMIT 25") or die $dbh->errstr;
        $sth->execute($user, $skipts) or die $dbh->errstr;
        my $count = $sth->rows;

	print "<h2>Stats for $user</h2>\n";
        print "<p>Click any table header to sort.</p>\n";
        print_table_header();

	while (my $row = $sth->fetchrow_hashref()) {
                print_row($row);
	}
        print_table_footer();
        if ($count >= 25) {
                print "<a href=\"stats.pl?user=$user&skipts=$skipts\">Next</a>\n";
        }
        print "<br />\n";
	$sth->finish;
}
else { # no username given, display other stats
        print "<h1>ADOM Server User Stats</h1>\n";
        # ask for an(other) user
        print '<p><form name="input" action="stats.pl"><strong>Enter username: </strong><input type="text" name="user"><input type="submit" value="Submit"></form></p>';
        my $statement = "SELECT * from stats WHERE reason<>'quit prematurely' AND reason<>'left the Drakalor Chain never to come back' AND date<=? ORDER BY date DESC LIMIT 10";
        my $sth = $dbh->prepare($statement) or die $dbh->errstr;
        $sth->execute($skipts) or die $dbh->errstr;
        my $count = $sth->rows;
        if ($skipts < $MAX_DATE) {
                print "<h2>$count interesting games ending " . (strftime "%F %T", localtime $skipts) . " or before</h2>\n";
        }
        else {
                print "<h2>Last $count interesting games</h2>\n";
        }
        print_table_header();
	while (my $row = $sth->fetchrow_hashref()) {
                print_row($row);
	}
        print_table_footer();
        if ($count >= 10) {
                print "<a href=\"stats.pl?skipts=$skipts\">Next</a>\n";
        }
	$sth->finish;
}
$dbh->disconnect;

print $cgi->end_html;
exit;

sub print_table_header() {
	print "<table class='sortable'><thead><tr>\n";
        my @headers = ("Name", "Score", "Lvl", "R /", "C", "Align", "Game Time", "Turns", "Result", "Bosses", "Date", "Version");
        foreach my $header (@headers) {
                if ($header eq "Game Time") { print "<th class='sorttable_numeric'>$header</th>"; }
                else { print "<th>$header</th>"; }
        }
        print "</tr></thead><tbody>\n";
}
sub print_table_footer() {
	print "</tbody></table>\n";
        print "<br />\n";
        if (defined($cgi->param('skipts'))) {
                print "<noscript>Back</noscript><script>document.write('<a href=\"' + document.referrer + '\">Back</a>');</script>";
        }
        print " <a href=\"stats.pl\">Home</a> ";
}
sub print_row($) {
        my $row = shift;
        print "<tr>";
        if (!defined($cgi->param('user'))) { # add username to char name on top list
                $user = $row->{'username'};
                print "<td class='left'>$row->{'name'} (<a class='small' href='/stats.pl?user=$user'>$user</a>)</td>";
        }
        else { # on user page, just print name
                print "<td class='left'>$row->{'name'}</td>";
        }
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
        print "<td sorttable_customkey='$row->{'date'}'>" . (strftime "%d %b '%y", localtime $row->{'date'}) . "</td>";
        print "<td>$row->{'version'}</td>";
        print "</tr>\n";

       $skipts = $row->{'date'};
}
