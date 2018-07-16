<?xml version="1.0" encoding="UTF-8"?>
<!--# Basic scene (map) text format
# Some are optional depending on the entity type, but should be specified in the given order
# Format = EntityTypeID shaderID [xpos ypos zpos] [size] [quality] [textureID]-->
<atlasscene name="Game.as">
	 <!-- <bgmusic> -->
		<!-- <sound file="01.mp3"/> -->
		<!-- <sound file="02.mp3"/> -->
		<!-- <sound file="03.mp3"/> -->
	<!-- </bgmusic> -->
	
	<sounds>
		<sound id="s_pip" file="pip01.wav"/>
		<sound id="s_select" file="porcSmall.wav"/>	
		<sound id="s_ok" file="ok.wav"/>	
		<sound id="s_no" file="no.wav"/>	
	</sounds>
	<!-- <script file="01.asc"/> -->
	
	<camera>
		<position x="5" y="15.0" z="-20.0"/>
		<target x="0.0" y="0.0" z="0.0"/>
	</camera>
	
	<lights>
		<ambient>
			<colour r="1.0" g="1.0" b="1.0"/>
		</ambient>
		<point id="1">
			<position x="0" y="5.0" z="0.0"/>
			<colour r="1.0" g="1.0" b="1.0"/>
		</point>
	</lights>
	
	<ui>
		<sprite id="s0" x="40" y="40" width="200" height="400">
			<texture name="shadowbox.png"/>
		</sprite>
		
		<text id="m_playerTurn" value="Starting Game..." x="55" y="80" style="xlarge"/>
		<text id="m_turnCounter" value="Turns: " x="60" y="200" style="large"/>
		<text id="m_redScore" value="Red Captures: " x="60" y="280" style="large"/>
		<text id="m_whiteScore" value="White Captures: " x="60" y="360" style="large"/>
	</ui>
	
	<entities>
		<origin/>
		
		<model id="tbl">
			<shader name="lighting"/>
			<position x="0" y="-29.5" z="0"/>
			<uniformscale value="6"/>
			<colour r="0.5" g="0.5" b="0.8"/>
			<mesh name="table.obj"/>
		</model>
		
		<plane id="flr">
			<shader name="littex"/>
			<position x="0" y="-32" z="0"/>
			<texture name="wall.jpg" repeat="20"/>
			<uniformscale value="30"/>
			<physics enable="false"/>
		</plane>
		
		<!-- Selection squares -->
		<plane id="red_overlay">
			<shader name="texture"/>
			<position x="-7" y="10" z="7"/>
			<texture name="red_highlight.png"/>
			<physics enable="false"/>
		</plane>
		<plane id="white_overlay">
			<shader name="texture"/>
			<position x="7" y="10" z="-7"/>
			<texture name="white_highlight.png"/>
			<physics enable="false"/>
		</plane>		
		
	
	</entities>
</atlasscene>




