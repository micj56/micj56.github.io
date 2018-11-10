<style type="text/css">
  .email>ins:before {content: "@"; text-decoration: none}
  .email del {text-decoration: none}
  .email>del {display: none}
  *:first-child+html .email>del {display:inline}  /* Workaround fuer IE 7 */
</style>

<script type="text/javascript">
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
</script>
