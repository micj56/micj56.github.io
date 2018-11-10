  var modDatum = new Date(document.lastModified);
  var tag = modDatum.getDate();
  var monat = modDatum.getMonth()+1;
  var jahr = modDatum.getFullYear();
  var stunde = modDatum.getHours();
  var minute = modDatum.getMinutes();
  if ( minute.length == 1 ) minute = "0" + minute;

  document.write(
    "<pre class=\"moddate\">\n\nStand:  "
    + tag + "." + monat + "." + jahr + "  "
    + stunde + ":" + minute);
