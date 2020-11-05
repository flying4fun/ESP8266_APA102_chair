#include <WString.h>

const char tab_manual[] PROGMEM = "\
<!DOCTYPE html>\
<html>\
\
<head>\
  <meta charset=\"utf-8\">\
    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\
  <title>RGB LED Night Light</title>\
  <link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\"\
   integrity=\"sha384-BVYiiSIFeK1dGmJRAkycuHAHRg32OmUcww7on3RYdg4Va+PmSTsz/K68vbdEjh4u\" crossorigin=\"anonymous\">\
  <style id=\"jsbin-css\">\
  body {\
    padding-top: 50px;\
  }\
  .body_container {\
    color: grey;\
    padding: 20px 15px;\
  }\
  </style>\
</head>\
\
<body style=\"background: #000000;\">\
  <form class=\"form-horizontal body_container\">\
    <div class=\"form-group\">\
      <label for=\"inputPower\" class=\"col-xs-2 control-label\">Power</label>\
      <div class=\"col-xs-9\">\
        <div class=\"btn-group btn-group-justified\" role=\"group\">\
          <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" class=\"btn btn-success btn-lg\" title=\"ON\" id=\"btnPowerOn\">ON</button>\
          </div>\
          <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" class=\"btn btn-danger btn-lg\" title=\"OFF\" id=\"btnPowerOff\">OFF</button>\
          </div>\
        </div>\
      </div>\
    </div>\
     <div class=\"form-group\">\
       <label for=\"inputProgram\" class=\"col-xs-2 control-label\">Program</label>\
       <div class=\"col-xs-9\">\
         <select class=\"form-control\" id=\"inputProgram\">\
           <option value=\"0\">Fade All</option>\
           <option value=\"1\">Snakey</option>\
           <option value=\"2\">Color Bands</option>\
           <option value=\"3\">Bouncing Ball</option>\
           <option value=\"4\">Flash USA</option>\
           <option value=\"5\">Flash Fade</option>\
           <option value=\"6\">Confetti</option>\
           <option value=\"7\">Blip-outs</option>\
           <option value=\"8\">Rainbow</option>\
           <option value=\"9\">Rainbow with Glitter</option>\
           <option value=\"10\">BPM</option>\
           <option value=\"11\">White</option>\
           <option value=\"12\">Cylon</option>\
           <option value=\"13\">Landing Light</option>\
           <option value=\"14\">Fireball</option>\
           <option value=\"15\">Pulse</option>\
           <option value=\"16\">Racer</option>\
           <option value=\"17\">Juggle</option>\
           <option value=\"18\">Bandcracker</option>\
           <option value=\"19\">Bandcracker 2</option>\
           <option value=\"20\">Police</option>\
           <option value=\"21\">Burst Fade</option>\
           <option value=\"22\">Drops Slide</option>\
           <option value=\"23\">Fire</option>\
         </select>\
       </div>\
     </div>\
   <div class=\"form-group\">\
     <label for=\"inputBrightness\" class=\"col-xs-2 control-label\">Brightness</label>\
     <div class=\"col-xs-9\">\
       <div class=\"input-group\">\
         <span class=\"input-group-addon\" id=\"spanBrightness\">50</span>\
         <input class=\"form-control\" id=\"inputBrightness\" type=\"range\" step=\"1\" min=\"0\" max=\"100\" />\
       </div>\
     </div>\
   </div>\
   <div class=\"form-group\">\
     <label for=\"inputValue\" class=\"col-xs-2 control-label\">Value</label>\
     <div class=\"col-xs-9\">\
       <div class=\"input-group\">\
         <span class=\"input-group-addon\" id=\"spanValue\">50</span>\
         <input class=\"form-control\" id=\"inputValue\" type=\"range\" step=\"1\" min=\"1\" max=\"16\" default=\"2\"/>\
       </div>\
     </div>\
   </div>\
   <div class=\"form-group\">\
     <label for=\"inputSpeed\" class=\"col-xs-2 control-label\">Speed</label>\
     <div class=\"col-xs-9\">\
       <div class=\"input-group\">\
         <span class=\"input-group-addon\" id=\"spanSpeed\">50</span>\
         <input class=\"form-control\" id=\"inputSpeed\" type=\"range\" step=\"10\" min=\"0\" max=\"100\" />\
       </div>\
     </div>\
   </div>\
   <div class=\"form-group\">\
     <label for=\"inputColor\" class=\"col-xs-2 control-label\">Color</label>\
   <div class=\"form-group\">\
     <div class=\"col-xs-9\">\
       <div class=\"btn-group btn-group-justified\" role=\"group\">\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonWhite\" class=\"btn btn-default btn-color\" style=\"background: #FFFFFF;\" title=\"White\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonRed\" class=\"btn btn-default btn-color\" style=\"background: #FF0000;\" title=\"Red\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonOrange\" class=\"btn btn-default btn-color\" style=\"background: #FF8000;\" title=\"Orange\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonYellow\" class=\"btn btn-default btn-color\" style=\"background: #FFFF00;\" title=\"Yellow\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonChartreuse\" class=\"btn btn-default btn-color\" style=\"background: #80FF00;\" title=\"Chartreuse\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonGreen\" class=\"btn btn-default btn-color\" style=\"background: #00FF00;\" title=\"Green\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonSpringGreen\" class=\"btn btn-default btn-color\" style=\"background: #00FF80;\" title=\"Spring Green\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonCyan\" class=\"btn btn-default btn-color\" style=\"background: #00FFFF;\" title=\"Cyan\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonAzure\" class=\"btn btn-default btn-color\" style=\"background: #0080FF;\" title=\"Azure\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonBlue\" class=\"btn btn-default btn-color\" style=\"background: #0000FF;\" title=\"Blue\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonViolet\" class=\"btn btn-default btn-color\" style=\"background: #8000FF;\" title=\"Violet\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonMagenta\" class=\"btn btn-default btn-color\" style=\"background: #FF00FF;\" title=\"Magenta\">&nbsp;</button>\
         </div>\
         <div class=\"btn-group\" role=\"group\">\
           <button type=\"button\" id=\"buttonRose\" class=\"btn btn-default btn-color\" style=\"background: #FF0080;\" title=\"Rose\">&nbsp;</button>\
         </div>\
       </div>\
       <div class=\"btn-group\" role=\"group\">\
         <input type=\"checkbox\" id=\"checkboxColorCycle\">Color Cycle<br>\
       </div>\
     </div>\
   </div>\
    <div class=\"form-group\">\
       <label for=\"inputStatus\" class=\"col-xs-2 control-label\">Status</label>\
       <div class=\"col-xs-9\">\
             <p id=\"status\" class=\"form-control-static\"></p>\
       </div>\
    </div>\
  </form>\
  <script src=\"https://ajax.googleapis.com/ajax/libs/jquery/1.12.4/jquery.min.js\"></script>\
  <script>window.jQuery || document.write('<script src=\"../../assets/js/vendor/jquery.min.js\"><\\/script>')</script>\
  <script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"\
   integrity=\"sha384-Tc5IQib027qvyjSMfHjOMaLkfuWVxZxUPnCJA7l2mCWNIpG9mGCD8wGNIcPD7Txa\"\
   crossorigin=\"anonymous\"></script>\
  <script type=\"text/javascript\">\
    var brightnessTimer = {};\
    var speedTimer = {};\
    var urlBase = \"\";\
    \
    $(\"#inputBrightness\").on(\"input\", function() {\
      $(\"#spanBrightness\").html($(this).val());\
      setBrightness($(\"#inputBrightness\").val());\
    });\
    $(\"#inputSpeed\").on(\"input\", function() {\
      $(\"#spanSpeed\").html($(this).val());\
      setSpeed($(\"#inputSpeed\").val());\
    });\
    $(\"#inputValue\").on(\"input\", function() {\
      $(\"#spanValue\").html($(this).val());\
      setValue($(\"#inputValue\").val());\
    });\
    $(\"#btnPowerOn\").click( function() {\
      setPower(1);\
    });\
    $(\"#btnPowerOff\").click( function() {\
      setPower(0);\
    });\
    $(\"#buttonWhite\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FFFFFF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonRed\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FF0000\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonOrange\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FF8000\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonYellow\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FFFF00\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonChartreuse\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"80FF00\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonGreen\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"00FF00\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonSpringGreen\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"00FF80\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonCyan\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"00FFFF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonAzure\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"0080FF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonBlue\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"0000FF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonViolet\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"8000FF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonMagenta\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FF00FF\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#buttonRose\").click( function() {\
      setColorCycle(\"0\");\
      setPalette(\"FF0080\");\
      $(\"input[type='checkbox']\").attr(\"checked\",false);\
    });\
    $(\"#checkboxColorCycle\").on(\"change\", function () {\
      setColorCycle($(\"#checkboxColorCycle\").is(\":checked\") ? 1 : 0);\
    });\
    $(\"#inputProgram\").on(\"change\", function() {\
      setProgram($(\"#inputProgram\").val());\
    });\
    function delaySetBrightness() {\
      clearTimeout(brightnessTimer);\
      brightnessTimer = setTimeout(function() {\
        setBrightness($(\"#inputBrightness\").val());\
      }, 100);\
    }\
    function setBrightness(value) {\
      $.post(urlBase + \"brightness\", \"value=\" + value, function(data) {\
        $(\"#status\").html(\"Set Brightness: \" + data);\
      });\
    }\
    function delaySetSpeed() {\
      clearTimeout(speedTimer);\
      speedTimer = setTimeout(function() {\
        setSpeed($(\"#inputSpeed\").val());\
      }, 100);\
      $(\"#status\").html(\"Set Speed: \" + data);\
    }\
    function setSpeed(value) {\
      $.post(urlBase + \"speed\", \"value=\" + value, function(data) {\
        $(\"#status\").html(\"Set Speed: \" + data);\
      });\
    }\
    function setValue(value) {\
      $.post(urlBase + \"value\", \"value=\" + value, function(data) {\
        $(\"#status\").html(\"Set Value: \" + data);\
      });\
    }\
    function setPalette(value) {\
      $.post(urlBase + \"palette\", \"value=\" + value, function(data) {\
        $(\"#status\").html(\"Set Palette: \" + data);\
      });\
    }\
    function setPower(value) {\
      $.post(urlBase + \"power\", \"value=\" + value + \"&brightness=\" +\
        $(\"#inputBrightness\").val(), function(data) {\
        $(\"#status\").html(\"Set Power: \" + data);\
      });\
    }\
    function setProgram(value) {\
      $.post(urlBase + \"program\", \"value=\" + value + \"&program=\" +\
        $(\"#inputProgram\").val(), function(data) {\
        $(\"#status\").html(\"Set Program: \" + data);\
      });\
    }\
    function setColorCycle(value) {\
      $.post(urlBase + \"colorcycle\", \"value=\" + value + \"&colorcycle=\" +\
        $(\"#checkboxColorCycle\").checked, function(data) {\
        $(\"#status\").html(\"Set Checkbox: \" + data);\
      });\
    }\
  </script>\
</body>\
</html>\
\r\n";
