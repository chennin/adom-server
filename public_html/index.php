<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.1//EN"
  "http://www.w3.org/TR/xhtml11/DTD/xhtml11.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en">
<?php 
$servername = 'Your.Address.Here';
$yourname = "Your name and contact info";
$twit = "Your Twitter handle";
?>

<head><title>Public ADOM Server: <?php echo $servername ?></title>
<meta http-equiv="Content-Type" content="text/html;charset=utf-8" />
<meta name="description" content="Information and instructions for this public ADOM Server">
<link rel="STYLESHEET" type="text/css" href="k.css" />
</head>
<body>

<h1><A NAME="0"><?php echo $servername?></A></h1>
<p>This is a public, session-recording <a href="http://adom.de">ADOM</a> (optionally <a href="http://kiserai.net/adom/sage/">Sage</a>) 1.1.1 / 1.2.0pX / 1.0.0 / Eternium Man SSH server.</p>
<p>It does not hurt to read through the following instructions before playing.</p>

</p>
<p>Follow on twitter: <a href="https://twitter.com/<?php echo $twit ?>">@<?php echo $twit ?></a></p>


<h1>Table of Contents</h1>
<UL>
<LI><A HREF="#news">News</A>
<LI><A HREF="#connect">Connecting</A>
<LI><A HREF="#play">Playing</A>
<LI><A HREF="#spectate">Spectating</A>   
<LI><A HREF="#ttyrec">Session recording and playback</A>
<LI><A HREF="#score">Hiscore</A>
<LI><A HREF="#config">Editing the config files</A>
<LI><A HREF="#putty">PuTTY Settings</A>
<LI><A HREF="#misc">Miscellaneous</A>
<LI><A HREF="#contact">Contact and Thanks</A></UL>

<h2><A NAME="news">News</A></h2>
<p> </p>

<h2><A NAME="connect">Connecting</A></h2>

 <p>Connect to <?php echo $servername ?>, default SSH port (22), with your SSH client.</p>

  <p>You must use the SSH2 protocol for SSH interaction! The server will not allow the use of the obsolete SSH1 version (PuTTY should use the version 2 by default).</p>
          
 <p>If you're a Windows user and don't have a SSH client, download one (PuTTY) from the offical download page <a href="http://www.chiark.greenend.org.uk/~sgtatham/putty/download.html">here</a>. PuTTY is a single executable and needs no installing.</p>

<p>Telnet is not available.</p>

<p>When you get <strong>"The server's host key is not cached in the registry"</strong>: this is a normal message the first time you connect.  Accept the host key by hitting 'yes'.

<h2><A NAME="play">Playing</A></h2>

  <p>To create a new user, give username 'adom' and password 'adom', follow directions, then re-log in with your newly created account.</p>

 <p>NOTE: only 3 registrations allowed per IP every day.</p>

  <p>Note that ADOM will not launch if your terminal size is something other than 80x25 characters!!</p>

  <p>The first time you launch ADOM, you may erroneously be told that your terminal is the wrong size.  This should go away upon subsequent launches if your terminal is 80x25.</p>

  <p>At the menu, you may choose whether you want to play ADOM 1.1.1, 1.2.0p3 1.0.0 or <a href="guidebook/challenge.html">Eternium Man</a> challenge and whether you want to use Sage or not. Note that 1.1.1/1.2.0p3/1.2.0p4+/1.0.0/Eternium Man have separate save- and hiscore files, and also that Sage is can be used with 1.2.0pX but may still be unstable. The server automatically enforces the rules of the Eternium Man challenge; breaking the rules will get your ADOM process killed.</p>
 <p>1.2.0p4 (and above) is available for people who donated to the <a href="http://www.indiegogo.com/resurrect-adom-development">ADOM Resurrection</a> campaign.  Simply select the menu option for 1.2.0pX and select a specific prerelease.  If you have not done this before, you will be prompted for the details from the e-mail you got entitled "ADOM 1.2.0p4 now available for ADOM: The Resurrection Donors".</p>
 <p>With 1.1.1 and 1.0.0, the server allows you to select your own birth sign and to reroll your stats until you are satisfied. Please use the stat roller gently.</p>

 <p>PROTIP:</p>

    <blockquote>In the game, there is a place called "The Bug-Infested Temple". Normally you will need at least a hundred deaths recorded to be allowed entry. However,  on the server, you will not need to reach this limit.</blockquote>

<h2><A NAME="spectate">Spectating</A></h2>

  <p>To spectate current players, log in with username 'spectator' and password 'spectator'. You may then choose the game to spectate, if there are players online.</p>
  <p>You can also spectate at the regular user menu by pressing 's', no password required.</p>

<h2><A NAME="ttyrec">Session recording and playback</A></h2>

  <p>Optionally, you can record every ADOM session. Press 'r' at the menu to toggle this feature.  It is off by default.</p>

  <p>You can play back any recorded game by logging in with the username 'ttyrecplay' and the password 'ttyrecplay'. Instructions for selecting and playing a recording are explained when you log in. </p>
  <p>The recordings are in "ttyrec" format, and can be downloaded from each user's WWW-accessable /ttyrecs directory if they've enabled WWW access).</p>

   <p>ttyrecs older than 30 days will be deleted.</p>

<h2><A NAME="score">Hiscore</A></h2>

   <p>The hiscore dumps of the server are available at the following address: </p>
   <blockquote><a href="adom_hiscore">http://<?php echo $servername?>/adom_hiscore/</a></blockquote>

  <p>On the IRC channel <a href="irc://irc.freenode.net/adom">#adom on Freenode</a>, there is the IRC-bot Fistanarius announcing new Hall of Fame (top 100) entries, if they exceed a certain number of points. </p>
  <p>Scores (above a higher threshold) are also announced on <a href="https://twitter.com/<?php echo $twit ?>">Twitter</a>.</p>
<h2><A NAME="config">Editing the config files</A></h2>

  <p>You are allowed to edit ADOM and ADOM Sage configuration files on the server. You can launch the nano editor (see main menu when you log in) for writing your own configurations.</p>

 <p>Key bindings are displayed at the bottom of the nano window. "^" means "control", so press ctrl+o to save and ctrl+x to exit.</p>

<h2><A NAME="putty">Recommended PuTTY Settings</a></h2>

<p>In PuTTY, the following settings are highly recommended:</p>

 <ul><li>Window -> Rows: 25 (you NEED this to launch ADOM!)</li>
  <li>Window -> Columns: 80 (this should be the default)</li>
  <li>Window -> "Forbid resizing completely" or "Change the size of the font"</li>
  <li>Window -> Appearance -> "Underline"</li>
  <li>Window -> Selection -> Windows (Middle extends, Right brings up menu) (prevents right-click from pasting)</li>
  <li>Terminal -> Bell -> None (bell disabled)</li>
  <li>Terminal -> Features -> Disable application keypad mode (this makes the numpad work)</li>
  <li>Terminal -> Keyboard -> xterm R6 (in order to use the function keys properly)</li>
  <li>Connection -> SSH -> Enable Compression</li>
  <li>Connection -> Enable TCP keepalives</li>
</ul>


  <p>You may want to change the default font, too. Note that you can save these settings, including username (Connection -> Auto-login username) in the main Session window.</p>

 <p>If you are used to the DOS or Windows versions of ADOM, it's possible to configure PuTTY to look like them; thanks to rmcin329 for finding this (Windows XP only?):</p>

        <table>
	<tr><td>ANSI black</td><td>0 0 0</td><td>ANSI bold black</td><td>119 119 119</td></tr>
        <tr><td>ANSI red:</td><td>255 68 0</td><td>ANSI bold red</td><td>255 119 0</td></tr>
        <tr><td>ANSI green</td><td>0 170 0</td><td>ANSI bold green</td><td>0 255 0</td></tr>
        <tr><td>ANSI yellow</td><td>153 85 0</td><td>ANSI bold yellow</td><td>221 221 0</td></tr>
        <tr><td>ANSI blue</td><td>0 0 170</td><td>ANSI bold blue</td><td>0 119 255</td></tr>
        <tr><td>ANSI magenta</td><td>170 0 170</td><td>ANSI bold magenta</td><td>255 0 255</td></tr>
        <tr><td>ANSI cyan</td><td>0 170 170</td><td>ANSI bold cyan</td><td>0 255 255</td></tr>
        <tr><td>ANSI white</td><td>187 187 187</td><td>ANSI bold white</td><td>255 255 255</td></tr></table>

         <p>If you want the cursor the same, use bold white for it, and under appearance set it to underline.</p>

<h2><A NAME="misc">Miscellaneous</A></h2>

  <p>There is a little problem with the terminal bell - spectators are able to
  send it. You should disable the bell from the playing/spectating terminal.</p>

 <p>If you use another terminal emulator see its documentation for setting colors, obviously.</p>

  <p>If the game happens to crash, there is a supposedly working restore system that allows you to restore the last save game if the ADOM process crashes.</p>

  <p>In #adom on Freenode, there is a bot named soradombot.  Use "@m monstername" for detailed concise info about a monster, and "@i itemname" for detailed concise info about an item.

  <p>Also, you can enable your user directory to be accessible (read-only) at http://<?php echo $servername?>/adom_users/. You can enable or disable this access any time you wish. Useful for exporting character dumps, configs, etc.!</p>

  <p>The source code for ADOM Sage, with Alucard's modifications, can be found on <a href="https://github.com/AlucardZero/adom-sage/">GitHub</a>.</p>

 <p>The source code for the star sign selector, and a version for Windows, is <a href="ss">available</a>.</p>
<h2><A NAME="contact">Contact and Thanks</A></h2>

  <p><?php echo $servername?> is run by <?php echo $yourname?></p>

  <p>Please report any trouble to #adom on Freenode.</p>

  <p>You might also want to visit the #adom channel in IRCNet, but then again you might not want to.  There is also #adom on Freenode (newer, smaller, and friendlier).</p>
  
  <p>Huge thanks to Chousuke, Vladimir Panteleev, jaakkos, and sorear for making all this possible!</p>

  <p>Sensible enhancement ideas are also welcome! Have fun playing!</p>

</body></html>
