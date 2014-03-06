#!/usr/bin/ploticus

#  Render with:
#    cat series.csv | ./altitude_plot.pl 

#set boxwidth = 18.5
#set boxheight = 3

#proc page
  pagesize: 20, 11

#proc getdata
  delim: comma
  fieldnameheader: yes
  commentchar: //
  command: ./extract.rb time altitudeToHoldTarget baroRawAltitude baroAltitude rangeFinderRange filteredAltitude targetVerticalSpeed baroAltitudeRate altitudeHoldThrottleCorrection altitudeThrottlePID_Ir
  # select: @@1 > 1074.1 && @@1 < 1091


// -------------- altitude --------------

#proc areadef
  box: @boxwidth @boxheight
  xautorange: datafield=time
  // yrange: 0 3
  // yrange: -3 -1
  yautorange: datafield=baroRawAltitude
  // areacolor: gray(0.8)
  location: 1 7.5

#proc yaxis
  label: altitude
  minortics: yes
  minorticinc: 1
  // grid: yes
  stubs: inc 1
  // stubrange: -1 15
  gridblocks: gray(0.9) white 

#proc xaxis
  stubs: inc 1
  // stubround: 100
  minortics: yes
  minorticinc: 0.1
  minorticlen: 0.04
  grid: color=gray(0.7) width=0.5
  #saveas x

#proc lineplot
  xfield: time
  yfield: altitudeToHoldTarget
  linedetails: color=green width=0.5
  stairstep: yes
  legendlabel: altitudeToHoldTarget
  clip: yes

#proc scatterplot
  xfield: time
  yfield: baroRawAltitude
  symbol: fillcolor=red style=fillonly shape=pixsquare radius=0.02
  legendlabel: baroRawAltitude

#proc lineplot
  xfield: time
  yfield: baroAltitude
  linedetails: color=blue width=0.5
  stairstep: yes
  legendlabel: baroAltitude
  clip: yes

#if 1 == 0
#proc lineplot
    xfield: time
    yfield: refiltered
    linedetails: color=orange width=0.5
    // stairstep: yes
    legendlabel: refiltered
    clip: yes
#endif


#proc legend


//--------------  vertical speed  --------------

#proc areadef
  box: @boxwidth @boxheight
  xautorange: datafield=time
  // yautorange: datafield=4
  yrange: -2 2
  location: 1 4.0

#proc yaxis
  label: speed
  // minortics: yes
  // minorticinc: 1
  // grid: yes
  stubs: inc 1
  // stubrange: -1 15
  gridblocks: gray(0.9) white 

#proc xaxis
  #clone x

#if 1 == 1
  #proc lineplot
    xfield: time
    yfield: baroAltitudeRate
    linedetails: color=blue width=0.5
    // stairstep: yes
    // fill: blue
    legendlabel: baroAltitudeRate
    clip: yes
#endif

#proc lineplot
  xfield: time
  yfield: targetVerticalSpeed
  linedetails: color=green width=0.5
  stairstep: yes
  legendlabel: targetVerticalSpeed
  clip: yes

#if 1 == 0
  #proc lineplot
    xfield: time
    yfield: refilteredRate
    linedetails: color=orange width=0.5
    // stairstep: yes
    legendlabel: refilteredRate
    clip: yes
#endif

#if 1 == 0
  #proc lineplot
    xfield: time
    yfield: refilteredRate2
    linedetails: color=purple width=0.5
    // stairstep: yes
    legendlabel: refilteredRate2
    clip: yes
#endif

#proc legend



//--------------  throttle  --------------

#proc areadef
  box: @boxwidth @boxheight
  xautorange: datafield=time
  # yautorange: datafield=5,6,7,8
  yrange: -150 150
  location: 1 0.5

#proc yaxis
  label: throttle
  // minortics: yes
  // minorticinc: 1
  // grid: yes
  stubs: inc 50
  // stubrange: -1 15
  gridblocks: gray(0.9) white 

#proc xaxis
  #clone x

#proc lineplot
  xfield: time
  yfield: altitudeHoldThrottleCorrection
  linedetails: color=black width=0.5
  stairstep: yes
  legendlabel: altitudeHoldThrottleCorrection
  clip: yes

#proc lineplot
  xfield: time
  yfield: altitudeThrottlePID_Ir
  linedetails: color=green width=0.5
  stairstep: yes
  legendlabel: altitudeThrottlePID_Ir
  clip: yes

#proc legend
  // location: 2, 2.5
