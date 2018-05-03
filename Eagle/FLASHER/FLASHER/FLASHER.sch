<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="8.2.2">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="14" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="13" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="14" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="13" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="11" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="50" name="dxf" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="53" name="tGND_GNDA" color="7" fill="9" visible="no" active="no"/>
<layer number="54" name="bGND_GNDA" color="1" fill="9" visible="no" active="no"/>
<layer number="56" name="wert" color="7" fill="1" visible="no" active="no"/>
<layer number="57" name="tCAD" color="7" fill="1" visible="no" active="no"/>
<layer number="59" name="tCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="60" name="bCarbon" color="7" fill="1" visible="no" active="no"/>
<layer number="90" name="Modules" color="5" fill="1" visible="yes" active="yes"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
<layer number="99" name="SpiceOrder" color="7" fill="1" visible="no" active="no"/>
<layer number="100" name="Muster" color="7" fill="1" visible="yes" active="yes"/>
<layer number="101" name="Patch_Top" color="12" fill="4" visible="yes" active="yes"/>
<layer number="102" name="Mittellin" color="7" fill="1" visible="yes" active="yes"/>
<layer number="103" name="Stiffner" color="7" fill="1" visible="yes" active="yes"/>
<layer number="104" name="Name" color="7" fill="1" visible="yes" active="yes"/>
<layer number="105" name="Beschreib" color="7" fill="1" visible="yes" active="yes"/>
<layer number="106" name="BGA-Top" color="7" fill="1" visible="yes" active="yes"/>
<layer number="107" name="BD-Top" color="7" fill="1" visible="yes" active="yes"/>
<layer number="108" name="tBridges" color="7" fill="1" visible="yes" active="yes"/>
<layer number="109" name="tBPL" color="7" fill="1" visible="yes" active="yes"/>
<layer number="110" name="bBPL" color="7" fill="1" visible="yes" active="yes"/>
<layer number="111" name="MPL" color="7" fill="1" visible="yes" active="yes"/>
<layer number="112" name="tSilk" color="7" fill="1" visible="yes" active="yes"/>
<layer number="113" name="ReferenceLS" color="7" fill="1" visible="no" active="no"/>
<layer number="116" name="Patch_BOT" color="9" fill="4" visible="yes" active="yes"/>
<layer number="118" name="Rect_Pads" color="7" fill="1" visible="no" active="no"/>
<layer number="121" name="sName" color="7" fill="1" visible="yes" active="yes"/>
<layer number="122" name="_bPlace" color="7" fill="1" visible="yes" active="yes"/>
<layer number="123" name="tTestmark" color="7" fill="1" visible="no" active="yes"/>
<layer number="124" name="bTestmark" color="7" fill="1" visible="no" active="yes"/>
<layer number="125" name="_tNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="126" name="_bNames" color="7" fill="1" visible="yes" active="yes"/>
<layer number="127" name="_tValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="128" name="_bValues" color="7" fill="1" visible="yes" active="yes"/>
<layer number="129" name="Mask" color="7" fill="1" visible="yes" active="yes"/>
<layer number="131" name="tAdjust" color="7" fill="1" visible="no" active="yes"/>
<layer number="132" name="bAdjust" color="7" fill="1" visible="no" active="yes"/>
<layer number="144" name="Drill_legend" color="7" fill="1" visible="yes" active="yes"/>
<layer number="150" name="Notes" color="7" fill="1" visible="yes" active="yes"/>
<layer number="151" name="HeatSink" color="7" fill="1" visible="yes" active="yes"/>
<layer number="152" name="_bDocu" color="7" fill="1" visible="yes" active="yes"/>
<layer number="153" name="FabDoc1" color="6" fill="1" visible="no" active="no"/>
<layer number="154" name="FabDoc2" color="2" fill="1" visible="no" active="no"/>
<layer number="155" name="FabDoc3" color="7" fill="15" visible="no" active="no"/>
<layer number="199" name="Contour" color="7" fill="1" visible="yes" active="yes"/>
<layer number="200" name="200bmp" color="1" fill="10" visible="yes" active="yes"/>
<layer number="201" name="201bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="202" name="202bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="203" name="203bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="204" name="204bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="205" name="205bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="206" name="206bmp" color="7" fill="10" visible="yes" active="yes"/>
<layer number="207" name="207bmp" color="8" fill="10" visible="yes" active="yes"/>
<layer number="208" name="208bmp" color="9" fill="10" visible="yes" active="yes"/>
<layer number="209" name="209bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="210" name="210bmp" color="7" fill="1" visible="no" active="yes"/>
<layer number="211" name="211bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="212" name="212bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="213" name="213bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="214" name="214bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="215" name="215bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="216" name="216bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="217" name="217bmp" color="18" fill="1" visible="no" active="no"/>
<layer number="218" name="218bmp" color="19" fill="1" visible="no" active="no"/>
<layer number="219" name="219bmp" color="20" fill="1" visible="no" active="no"/>
<layer number="220" name="220bmp" color="21" fill="1" visible="no" active="no"/>
<layer number="221" name="221bmp" color="22" fill="1" visible="no" active="no"/>
<layer number="222" name="222bmp" color="23" fill="1" visible="no" active="no"/>
<layer number="223" name="223bmp" color="24" fill="1" visible="no" active="no"/>
<layer number="224" name="224bmp" color="25" fill="1" visible="no" active="no"/>
<layer number="225" name="225bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="226" name="226bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="227" name="227bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="228" name="228bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="229" name="229bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="230" name="230bmp" color="7" fill="1" visible="yes" active="yes"/>
<layer number="231" name="Eagle3D_PG1" color="7" fill="1" visible="no" active="no"/>
<layer number="232" name="Eagle3D_PG2" color="7" fill="1" visible="no" active="no"/>
<layer number="233" name="Eagle3D_PG3" color="7" fill="1" visible="no" active="no"/>
<layer number="248" name="Housing" color="7" fill="1" visible="yes" active="yes"/>
<layer number="249" name="Edge" color="7" fill="1" visible="yes" active="yes"/>
<layer number="250" name="Descript" color="7" fill="1" visible="yes" active="yes"/>
<layer number="251" name="SMDround" color="7" fill="1" visible="yes" active="yes"/>
<layer number="254" name="cooling" color="7" fill="1" visible="yes" active="yes"/>
<layer number="255" name="routoute" color="7" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="microbuilder">
<description>&lt;h2&gt;&lt;b&gt;microBuilder.eu&lt;/b&gt; Eagle Footprint Library&lt;/h2&gt;

&lt;p&gt;Footprints for common components used in our projects and products.  This is the same library that we use internally, and it is regularly updated.  The newest version can always be found at &lt;b&gt;www.microBuilder.eu&lt;/b&gt;.  If you find this library useful, please feel free to purchase something from our online store. Please also note that all holes are optimised for metric drill bits!&lt;/p&gt;

&lt;h3&gt;Obligatory Warning&lt;/h3&gt;
&lt;p&gt;While it probably goes without saying, there are no guarantees that the footprints or schematic symbols in this library are flawless, and we make no promises of fitness for production, prototyping or any other purpose. These libraries are provided for information puposes only, and are used at your own discretion.  While we make every effort to produce accurate footprints, and many of the items found in this library have be proven in production, we can't make any promises of suitability for a specific purpose. If you do find any errors, though, please feel free to contact us at www.microbuilder.eu to let us know about it so that we can update the library accordingly!&lt;/p&gt;

&lt;h3&gt;License&lt;/h3&gt;
&lt;p&gt;This work is placed in the public domain, and may be freely used for commercial and non-commercial work with the following conditions:&lt;/p&gt;
&lt;p&gt;THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INSTITUTE OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
&lt;/p&gt;</description>
<packages>
<package name="2X05_1.27MM_SMT">
<description>&lt;p&gt;2x05 1.27mm Headers SMT (2.0mm Height)&lt;/p&gt;
4UConnector: 16846</description>
<wire x1="-5.27" y1="3.5" x2="5.27" y2="3.5" width="0.2032" layer="21"/>
<wire x1="5.27" y1="3.5" x2="5.27" y2="-3.5" width="0.2032" layer="21"/>
<wire x1="5.27" y1="-3.5" x2="1" y2="-3.5" width="0.2032" layer="21"/>
<wire x1="1" y1="-3.5" x2="-1" y2="-3.5" width="0.2032" layer="21"/>
<wire x1="-1" y1="-3.5" x2="-5.27" y2="-3.5" width="0.2032" layer="21"/>
<wire x1="-5.27" y1="-3.5" x2="-5.27" y2="3.5" width="0.2032" layer="21"/>
<wire x1="-1" y1="-3.5" x2="-1" y2="-4" width="0.2032" layer="21"/>
<wire x1="-1" y1="-4" x2="1" y2="-4" width="0.2032" layer="21"/>
<wire x1="1" y1="-4" x2="1" y2="-3.5" width="0.2032" layer="21"/>
<smd name="2" x="-2.54" y="1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="4" x="-1.27" y="1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="6" x="0" y="1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="8" x="1.27" y="1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="10" x="2.54" y="1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="1" x="-2.54" y="-1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="3" x="-1.27" y="-1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="5" x="0" y="-1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="7" x="1.27" y="-1.85" dx="0.65" dy="2.6" layer="1"/>
<smd name="9" x="2.54" y="-1.85" dx="0.65" dy="2.6" layer="1"/>
<text x="-5.199" y="3.806" size="0.8128" layer="25" ratio="18">&gt;NAME</text>
<text x="-5.199" y="-4.2465" size="0.4064" layer="27" ratio="10">&gt;VALUE</text>
<polygon width="0.127" layer="21">
<vertex x="1.016" y="-3.556"/>
<vertex x="1.016" y="-3.937"/>
<vertex x="-1.016" y="-3.937"/>
<vertex x="-1.016" y="-3.556"/>
</polygon>
</package>
<package name="2X05_1.27MM_BOX_NOPOSTS">
<description>&lt;p&gt;4UCon: 19735 (with cap), 15117 (without caps)&lt;/p&gt;</description>
<smd name="2" x="-2.54" y="1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="4" x="-1.27" y="1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="6" x="0" y="1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="8" x="1.27" y="1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="10" x="2.54" y="1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="1" x="-2.54" y="-1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="3" x="-1.27" y="-1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="5" x="0" y="-1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="7" x="1.27" y="-1.95" dx="0.76" dy="2.4" layer="1"/>
<smd name="9" x="2.54" y="-1.95" dx="0.76" dy="2.4" layer="1"/>
<text x="-6.6" y="3" size="0.8128" layer="25" ratio="18">&gt;NAME</text>
<text x="-6.1" y="-2.4" size="0.4064" layer="27" ratio="10">&gt;VALUE</text>
<wire x1="-6.45" y1="2.6" x2="6.45" y2="2.6" width="0.127" layer="51"/>
<wire x1="6.45" y1="2.6" x2="6.45" y2="-2.6" width="0.127" layer="51"/>
<wire x1="-6.45" y1="-2.6" x2="-6.45" y2="2.6" width="0.127" layer="51"/>
<wire x1="-1.2" y1="-2.6" x2="-6.45" y2="-2.6" width="0.127" layer="51"/>
<wire x1="6.45" y1="-2.6" x2="1.2" y2="-2.6" width="0.127" layer="51"/>
<wire x1="-5.575" y1="1.8" x2="5.575" y2="1.8" width="0.127" layer="51"/>
<wire x1="5.575" y1="1.8" x2="5.575" y2="-1.8" width="0.127" layer="51"/>
<wire x1="-5.575" y1="-1.8" x2="-5.575" y2="1.8" width="0.127" layer="51"/>
<wire x1="-1.2" y1="-1.8" x2="-5.575" y2="-1.8" width="0.127" layer="51"/>
<wire x1="5.575" y1="-1.8" x2="1.2" y2="-1.8" width="0.127" layer="51"/>
<wire x1="-1.2" y1="-1.8" x2="-1.2" y2="-2.6" width="0.127" layer="51"/>
<wire x1="1.2" y1="-1.8" x2="1.2" y2="-2.6" width="0.127" layer="51"/>
<polygon width="0.127" layer="21">
<vertex x="-6.5" y="-4"/>
<vertex x="-5.5" y="-4"/>
<vertex x="-6" y="-3"/>
</polygon>
<wire x1="-3.2" y1="2.7" x2="-6.5" y2="2.7" width="0.127" layer="21"/>
<wire x1="-6.5" y1="2.7" x2="-6.6" y2="2.7" width="0.127" layer="21"/>
<wire x1="-6.6" y1="2.7" x2="-6.6" y2="2.6" width="0.127" layer="21"/>
<wire x1="-6.6" y1="2.6" x2="-6.6" y2="-2.7" width="0.127" layer="21"/>
<wire x1="-6.6" y1="-2.7" x2="-3.2" y2="-2.7" width="0.127" layer="21"/>
<wire x1="3.2" y1="2.7" x2="6.6" y2="2.7" width="0.127" layer="21"/>
<wire x1="6.6" y1="2.7" x2="6.6" y2="-2.7" width="0.127" layer="21"/>
<wire x1="6.6" y1="-2.7" x2="3.2" y2="-2.7" width="0.127" layer="21"/>
</package>
<package name="2X05_1.27MM_BOX_POSTS">
<description>&lt;p&gt;4UCon: 20317&lt;/p&gt;</description>
<smd name="2" x="-2.54" y="2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="4" x="-1.27" y="2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="6" x="0" y="2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="8" x="1.27" y="2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="10" x="2.54" y="2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="1" x="-2.54" y="-2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="3" x="-1.27" y="-2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="5" x="0" y="-2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="7" x="1.27" y="-2.15" dx="0.76" dy="2.6" layer="1"/>
<smd name="9" x="2.54" y="-2.15" dx="0.76" dy="2.6" layer="1"/>
<text x="-6.6" y="3" size="0.8128" layer="25" ratio="18">&gt;NAME</text>
<text x="-6.1" y="-2.4" size="0.4064" layer="27" ratio="10">&gt;VALUE</text>
<wire x1="-6.275" y1="2.5" x2="6.275" y2="2.5" width="0.127" layer="51"/>
<wire x1="6.275" y1="2.5" x2="6.275" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-6.275" y1="-2.5" x2="-6.275" y2="2.5" width="0.127" layer="51"/>
<wire x1="-1.25" y1="-2.5" x2="-6.275" y2="-2.5" width="0.127" layer="51"/>
<wire x1="6.275" y1="-2.5" x2="1.25" y2="-2.5" width="0.127" layer="51"/>
<wire x1="-5.425" y1="1.65" x2="5.425" y2="1.65" width="0.127" layer="51"/>
<wire x1="5.425" y1="1.65" x2="5.425" y2="-1.65" width="0.127" layer="51"/>
<wire x1="-5.425" y1="-1.65" x2="-5.425" y2="1.65" width="0.127" layer="51"/>
<wire x1="-1.25" y1="-1.65" x2="-5.425" y2="-1.65" width="0.127" layer="51"/>
<wire x1="5.425" y1="-1.65" x2="1.25" y2="-1.65" width="0.127" layer="51"/>
<wire x1="-1.25" y1="-1.65" x2="-1.25" y2="-2.5" width="0.127" layer="51"/>
<wire x1="1.25" y1="-1.65" x2="1.25" y2="-2.5" width="0.127" layer="51"/>
<polygon width="0.127" layer="21">
<vertex x="-6.5" y="-4"/>
<vertex x="-5.5" y="-4"/>
<vertex x="-6" y="-3"/>
</polygon>
<wire x1="-3.2" y1="2.7" x2="-6.5" y2="2.7" width="0.127" layer="21"/>
<wire x1="-6.5" y1="2.7" x2="-6.5" y2="-2.7" width="0.127" layer="21"/>
<wire x1="-6.5" y1="-2.7" x2="-3.2" y2="-2.7" width="0.127" layer="21"/>
<wire x1="3.2" y1="2.7" x2="6.5" y2="2.7" width="0.127" layer="21"/>
<wire x1="6.5" y1="2.7" x2="6.5" y2="-2.7" width="0.127" layer="21"/>
<wire x1="6.5" y1="-2.7" x2="3.2" y2="-2.7" width="0.127" layer="21"/>
<hole x="-1.905" y="0" drill="1"/>
<hole x="1.905" y="0" drill="1"/>
</package>
</packages>
<symbols>
<symbol name="JTAG-CORTEX">
<wire x1="-6.35" y1="7.62" x2="6.35" y2="7.62" width="0.4064" layer="94"/>
<wire x1="6.35" y1="-7.62" x2="6.35" y2="7.62" width="0.4064" layer="94"/>
<wire x1="6.35" y1="-7.62" x2="-6.35" y2="-7.62" width="0.4064" layer="94"/>
<wire x1="-6.35" y1="7.62" x2="-6.35" y2="-7.62" width="0.4064" layer="94"/>
<circle x="-4.445" y="5.08" radius="0.635" width="0.254" layer="94"/>
<circle x="-4.445" y="2.54" radius="0.635" width="0.254" layer="94"/>
<circle x="-4.445" y="0" radius="0.635" width="0.254" layer="94"/>
<circle x="-4.445" y="-2.54" radius="0.635" width="0.254" layer="94"/>
<circle x="-4.445" y="-5.08" radius="0.635" width="0.254" layer="94"/>
<circle x="4.445" y="5.08" radius="0.635" width="0.254" layer="94"/>
<circle x="4.445" y="2.54" radius="0.635" width="0.254" layer="94"/>
<circle x="4.445" y="0" radius="0.635" width="0.254" layer="94"/>
<circle x="4.445" y="-2.54" radius="0.635" width="0.254" layer="94"/>
<circle x="4.445" y="-5.08" radius="0.635" width="0.254" layer="94"/>
<text x="-6.35" y="8.255" size="1.27" layer="95">&gt;NAME</text>
<text x="1.524" y="-4.318" size="0.635" layer="95">nRESET</text>
<text x="1.778" y="-1.778" size="0.635" layer="95">NC/TDI</text>
<text x="1.016" y="0.762" size="0.635" layer="95">SWO/TDO</text>
<text x="-0.762" y="3.302" size="0.635" layer="95">SWDCLK/TCK</text>
<text x="0" y="5.842" size="0.635" layer="95">SWDIO/TMS</text>
<text x="-5.588" y="6.096" size="0.635" layer="95">VCC</text>
<text x="-5.588" y="3.302" size="0.635" layer="95">GND</text>
<text x="-5.588" y="0.762" size="0.635" layer="95">GND</text>
<text x="-5.588" y="-1.778" size="0.635" layer="95">KEY</text>
<text x="-5.588" y="-4.318" size="0.635" layer="95">GNDDetect</text>
<text x="-6.35" y="-9.525" size="1.27" layer="96">&gt;VALUE</text>
<pin name="VCC" x="-7.62" y="5.08" visible="off" length="short" direction="pwr" swaplevel="1"/>
<pin name="SWDIO" x="7.62" y="5.08" visible="off" length="short" swaplevel="1" rot="R180"/>
<pin name="GND@1" x="-7.62" y="2.54" visible="off" length="short" direction="pwr" swaplevel="1"/>
<pin name="SWDCLK" x="7.62" y="2.54" visible="off" length="short" swaplevel="1" rot="R180"/>
<pin name="GND@2" x="-7.62" y="0" visible="off" length="short" direction="pwr" swaplevel="1"/>
<pin name="SWO" x="7.62" y="0" visible="off" length="short" swaplevel="1" rot="R180"/>
<pin name="KEY" x="-7.62" y="-2.54" visible="off" length="short" swaplevel="1"/>
<pin name="TDI" x="7.62" y="-2.54" visible="off" length="short" swaplevel="1" rot="R180"/>
<pin name="GNDDETECT" x="-7.62" y="-5.08" visible="off" length="short" swaplevel="1"/>
<pin name="NRESET" x="7.62" y="-5.08" visible="off" length="short" swaplevel="1" rot="R180"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="JTAG-CORTEX" prefix="X" uservalue="yes">
<description>&lt;p&gt;&lt;b&gt;Serial Wire (SW-DP) Connector&lt;/b&gt;&lt;/p&gt;
&lt;p&gt;Standard 0.05" 10-pin connector is for use with the Cortex M and Cortex A SWD (serial wire debugger) interface (SW-DP)&lt;/p&gt;
&lt;b&gt;Connectors&lt;/b&gt;
&lt;ul&gt;
&lt;li&gt;&lt;b&gt;2x05_1.27MM_SMT&lt;/b&gt; - Inexpensive surface-mount header with bare pins, compatible with standard 10-pin SWD cables (4UCON: 16846)&lt;/li&gt;
&lt;li&gt;&lt;b&gt;2x05_1.27MM_BOX_NOPOSTS&lt;/b&gt; - Polarised surface-mount box (4UCON with caps: 19735, without caps: 15117&lt;/li&gt;
&lt;li&gt;&lt;b&gt;2x05_1.27MM_BOX_POSTS&lt;/b&gt; - Polarised surface-mount box with mounting holes (4UCON: 20317)&lt;/li&gt;
&lt;/ul&gt;
&lt;b&gt;Pinout:&lt;/b&gt;
&lt;ul&gt;
&lt;li&gt;http://infocenter.arm.com/help/topic/com.arm.doc.faqs/attached/13634/cortex_debug_connectors.pdf&lt;/li&gt;
&lt;li&gt;http://www.keil.com/peripherals/coresight/connectors.asp&lt;/li&gt;
&lt;/ul&gt;</description>
<gates>
<gate name="G$1" symbol="JTAG-CORTEX" x="0" y="0"/>
</gates>
<devices>
<device name="" package="2X05_1.27MM_SMT">
<connects>
<connect gate="G$1" pin="GND@1" pad="3"/>
<connect gate="G$1" pin="GND@2" pad="5"/>
<connect gate="G$1" pin="GNDDETECT" pad="9"/>
<connect gate="G$1" pin="KEY" pad="7"/>
<connect gate="G$1" pin="NRESET" pad="10"/>
<connect gate="G$1" pin="SWDCLK" pad="4"/>
<connect gate="G$1" pin="SWDIO" pad="2"/>
<connect gate="G$1" pin="SWO" pad="6"/>
<connect gate="G$1" pin="TDI" pad="8"/>
<connect gate="G$1" pin="VCC" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="BOX" package="2X05_1.27MM_BOX_NOPOSTS">
<connects>
<connect gate="G$1" pin="GND@1" pad="3"/>
<connect gate="G$1" pin="GND@2" pad="5"/>
<connect gate="G$1" pin="GNDDETECT" pad="9"/>
<connect gate="G$1" pin="KEY" pad="7"/>
<connect gate="G$1" pin="NRESET" pad="10"/>
<connect gate="G$1" pin="SWDCLK" pad="4"/>
<connect gate="G$1" pin="SWDIO" pad="2"/>
<connect gate="G$1" pin="SWO" pad="6"/>
<connect gate="G$1" pin="TDI" pad="8"/>
<connect gate="G$1" pin="VCC" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
<device name="BOXPOSTS" package="2X05_1.27MM_BOX_POSTS">
<connects>
<connect gate="G$1" pin="GND@1" pad="3"/>
<connect gate="G$1" pin="GND@2" pad="5"/>
<connect gate="G$1" pin="GNDDETECT" pad="9"/>
<connect gate="G$1" pin="KEY" pad="7"/>
<connect gate="G$1" pin="NRESET" pad="10"/>
<connect gate="G$1" pin="SWDCLK" pad="4"/>
<connect gate="G$1" pin="SWDIO" pad="2"/>
<connect gate="G$1" pin="SWO" pad="6"/>
<connect gate="G$1" pin="TDI" pad="8"/>
<connect gate="G$1" pin="VCC" pad="1"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
<library name="FLASHER">
<packages>
<package name="SPRING_LOADED_HEADER">
<pad name="RST" x="2.54" y="0" drill="0.8" shape="square"/>
<pad name="VIN" x="0" y="0" drill="0.8" shape="square"/>
<pad name="SWDIO" x="5.08" y="0" drill="0.8" shape="square"/>
<pad name="SWCLK" x="7.62" y="0" drill="0.8" shape="square"/>
<pad name="GND" x="10.16" y="0" drill="0.8" shape="square"/>
<hole x="0" y="-3.76" drill="1.45"/>
<hole x="10.16" y="-3.76" drill="1.45"/>
<wire x1="-1.27" y1="-3.28" x2="-1.27" y2="-7.47" width="0.127" layer="21"/>
<wire x1="11.43" y1="-7.47" x2="11.43" y2="-3.28" width="0.127" layer="21"/>
<wire x1="11.43" y1="-3.28" x2="-1.27" y2="-3.28" width="0.127" layer="21"/>
<wire x1="-1.27" y1="-7.47" x2="11.43" y2="-7.47" width="0.127" layer="21"/>
<wire x1="0" y1="-2.54" x2="0" y2="-1.27" width="0.127" layer="21"/>
<wire x1="2.54" y1="-2.54" x2="2.54" y2="-1.27" width="0.127" layer="21"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="-1.27" width="0.127" layer="21"/>
<wire x1="7.62" y1="-2.54" x2="7.62" y2="-1.27" width="0.127" layer="21"/>
<wire x1="10.16" y1="-2.54" x2="10.16" y2="-1.27" width="0.127" layer="21"/>
</package>
</packages>
<symbols>
<symbol name="SPRING_LOADED_HEADER">
<pin name="GND" x="0" y="0" length="middle"/>
<pin name="SWCLK" x="0" y="2.54" length="middle"/>
<pin name="SWDIO" x="0" y="5.08" length="middle"/>
<pin name="RST" x="0" y="7.62" length="middle"/>
<pin name="VIN" x="0" y="10.16" length="middle"/>
<wire x1="5.08" y1="12.7" x2="15.24" y2="12.7" width="0.254" layer="94"/>
<wire x1="15.24" y1="12.7" x2="15.24" y2="-2.54" width="0.254" layer="94"/>
<wire x1="15.24" y1="-2.54" x2="5.08" y2="-2.54" width="0.254" layer="94"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="12.7" width="0.254" layer="94"/>
</symbol>
</symbols>
<devicesets>
<deviceset name="SPRING_LOADED_HEADER">
<gates>
<gate name="G$1" symbol="SPRING_LOADED_HEADER" x="2.54" y="5.08"/>
</gates>
<devices>
<device name="" package="SPRING_LOADED_HEADER">
<connects>
<connect gate="G$1" pin="GND" pad="GND"/>
<connect gate="G$1" pin="RST" pad="RST"/>
<connect gate="G$1" pin="SWCLK" pad="SWCLK"/>
<connect gate="G$1" pin="SWDIO" pad="SWDIO"/>
<connect gate="G$1" pin="VIN" pad="VIN"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0" drill="0">
</class>
</classes>
<parts>
<part name="X5" library="microbuilder" deviceset="JTAG-CORTEX" device="BOXPOSTS" value="4u20317">
<attribute name="MPN" value="3220-10-0300-00"/>
</part>
<part name="U$1" library="FLASHER" deviceset="SPRING_LOADED_HEADER" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="X5" gate="G$1" x="25.4" y="33.02">
<attribute name="MPN" x="10.16" y="48.26" size="1.778" layer="96" display="both"/>
</instance>
<instance part="U$1" gate="G$1" x="53.34" y="27.94"/>
</instances>
<busses>
</busses>
<nets>
<net name="N$2" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="RST"/>
<wire x1="53.34" y1="35.56" x2="40.64" y2="35.56" width="0.1524" layer="91"/>
<pinref part="X5" gate="G$1" pin="NRESET"/>
<wire x1="33.02" y1="27.94" x2="40.64" y2="27.94" width="0.1524" layer="91"/>
<wire x1="40.64" y1="27.94" x2="40.64" y2="35.56" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$3" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="SWDIO"/>
<pinref part="X5" gate="G$1" pin="SWDIO"/>
<wire x1="33.02" y1="38.1" x2="38.1" y2="38.1" width="0.1524" layer="91"/>
<wire x1="38.1" y1="38.1" x2="38.1" y2="33.02" width="0.1524" layer="91"/>
<wire x1="38.1" y1="33.02" x2="53.34" y2="33.02" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$4" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="SWCLK"/>
<pinref part="X5" gate="G$1" pin="SWDCLK"/>
<wire x1="33.02" y1="35.56" x2="35.56" y2="35.56" width="0.1524" layer="91"/>
<wire x1="35.56" y1="35.56" x2="35.56" y2="30.48" width="0.1524" layer="91"/>
<wire x1="35.56" y1="30.48" x2="53.34" y2="30.48" width="0.1524" layer="91"/>
</segment>
</net>
<net name="N$5" class="0">
<segment>
<pinref part="U$1" gate="G$1" pin="GND"/>
<wire x1="53.34" y1="27.94" x2="45.72" y2="27.94" width="0.1524" layer="91"/>
<wire x1="45.72" y1="27.94" x2="45.72" y2="22.86" width="0.1524" layer="91"/>
<pinref part="X5" gate="G$1" pin="GND@1"/>
<wire x1="17.78" y1="35.56" x2="15.24" y2="35.56" width="0.1524" layer="91"/>
<wire x1="15.24" y1="35.56" x2="15.24" y2="33.02" width="0.1524" layer="91"/>
<pinref part="X5" gate="G$1" pin="GND@2"/>
<wire x1="15.24" y1="33.02" x2="17.78" y2="33.02" width="0.1524" layer="91"/>
<pinref part="X5" gate="G$1" pin="GNDDETECT"/>
<wire x1="17.78" y1="27.94" x2="15.24" y2="27.94" width="0.1524" layer="91"/>
<wire x1="15.24" y1="27.94" x2="15.24" y2="33.02" width="0.1524" layer="91"/>
<junction x="15.24" y="33.02"/>
<wire x1="45.72" y1="22.86" x2="15.24" y2="22.86" width="0.1524" layer="91"/>
<wire x1="15.24" y1="22.86" x2="15.24" y2="27.94" width="0.1524" layer="91"/>
<junction x="15.24" y="27.94"/>
</segment>
</net>
<net name="N$6" class="0">
<segment>
<pinref part="X5" gate="G$1" pin="VCC"/>
<wire x1="17.78" y1="38.1" x2="12.7" y2="38.1" width="0.1524" layer="91"/>
<wire x1="12.7" y1="38.1" x2="12.7" y2="43.18" width="0.1524" layer="91"/>
<pinref part="U$1" gate="G$1" pin="VIN"/>
<wire x1="53.34" y1="38.1" x2="45.72" y2="38.1" width="0.1524" layer="91"/>
<wire x1="12.7" y1="43.18" x2="45.72" y2="43.18" width="0.1524" layer="91"/>
<wire x1="45.72" y1="43.18" x2="45.72" y2="38.1" width="0.1524" layer="91"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
