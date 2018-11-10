function noSpam() {
  var a = document.getElementsByTagName("a");
  for (var i = a.length-1; i >= 0; i--) {
    if (a[i].className.search(/\bemail\b/) != -1) {
      var email = a[i].firstChild.data + "@" + a[i].lastChild.data;
      a[i].innerHTML = email;
      a[i].href = "mailto:" + email;
    }
  }
}

function insertModificationDate() {

  var modDatum = new Date(document.lastModified);
  var tag = modDatum.getDate();
  var monat = modDatum.getMonth()+1;
  var jahr = modDatum.getFullYear();
  var stunde = modDatum.getHours();
  var minute = modDatum.getMinutes();
  var ausgabebereich = document.getElementById("modificationDate");

  if ( minute.length == 1 ) minute = "0" + minute;

  ausgabebereich.innerHTML =
    "<pre class=\"moddate\">\n\nStand:  "
    + tag + "." + monat + "." + jahr + "  "
    + stunde + ":" + minute;
}

function homepageInit() {
  noSpam();
  insertModificationDate();
}


