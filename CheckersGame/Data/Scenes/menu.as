<?xml version="1.0" encoding="UTF-8"?>
<!--# Basic scene (map) text format
# Some are optional depending on the entity type, but should be specified in the given order
# Format = EntityTypeID shaderID [xpos ypos zpos] [size] [quality] [textureID]-->
<atlasscene name="01.as">
	 <bgmusic>
		<sound file="menu.mp3"/>
	</bgmusic>
	
	<sounds>
		<sound id="s_pip" file="pip01.wav"/>
		<sound id="s_select" file="porcSmall.wav"/>	
	</sounds>
	<!-- <script file="01.asc"/> -->
	
	<camera>
		<position x="0" y="15.0" z="-20.0"/>
		<target x="0" y="0.0" z="0.0"/>
	</camera>
		
	<!-- <sky name="muddysky"/> -->
	
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
		<sprite id="s0" x="400" y="260" width="225" height="325">
			<texture name="shadowbox.png"/>
		</sprite>
		
		<text id="m_title" value="Atlas Draughts" horizontalalignment="centre" y="100" style="title"/>
		
		<text id="m_newresume" value="New Game" horizontalalignment="centre" y="320" style="xlarge"/>
		<text id="m_loadgame" value="Load Game" horizontalalignment="centre" y="400" style="xlarge"/>
		<text id="m_help" value="Help" horizontalalignment="centre" y="480" style="xlarge"/>
		<text id="m_exit" value="Exit" horizontalalignment="centre" y="560" style="xlarge"/>
		
		<!-- help text items -->
		<sprite id="s1" x="220" y="260" width="590" height="325" visible="false">
			<texture name="shadowbox.png"/>
		</sprite>
		<text id="m_helpText1" value="Welcome to Atlas Draughts!" horizontalalignment="centre" y="320" style="xlarge" visible="false"/>
		<text id="m_helpText2" value="This game has been created as a tech demo for the Atlas engine." horizontalalignment="centre" y="350" style="medium" visible="false"/>
		<text id="m_helpText3" value="Basic controls" horizontalalignment="centre" y="380" style="large" visible="false"/>
		<text id="m_helpText4" value="Use the ARROW KEYS to select menu items and to move your cursor" horizontalalignment="centre" y="400" style="medium" visible="false"/>
		<text id="m_helpText5" value="Hold SHIFT while pressing the ARROW KEYS to move the camera" horizontalalignment="centre" y="420" style="medium" visible="false"/>
		<text id="m_helpText6" value="Press SPACE to select a piece and a destination square" horizontalalignment="centre" y="440" style="medium" visible="false"/>
		<text id="m_helpText7" value="Your turn will complete when you make a move, or you can press ENTER to forfeit your turn." horizontalalignment="centre" y="460" style="medium" visible="false"/>
		
	</ui>
	
	<entities>
		<origin/>
		
		<!--<model id="tbl">
			<shader name="lighting"/>
			<position x="0" y="-29.5" z="0"/>
			<uniformscale value="6"/>
			<colour r="0.5" g="0.5" b="0.8"/>
			<mesh name="table.obj"/>
		</model>-->
		
		<plane id="flr">
			<shader name="littex"/>
			<position x="0" y="-32" z="0"/>
			<texture name="wall.jpg" repeat="20"/>
			<uniformscale value="30"/>
			<physics enable="false"/>
		</plane>

	</entities>
</atlasscene>




