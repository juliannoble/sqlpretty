jQuery(document).ready(function() {
   jQuery('.sqlpretty-result').hide();
   jQuery('.sqlpretty-hide-show').each(function(idx) {
       var lnkname = jQuery(this).html()
       jQuery(this).html('Show ' + lnkname);
       jQuery(this).click(function() {
           var debugdiv = jQuery(this).parent().parent();
           var resdiv = debugdiv.find('.sqlpretty-result');
           var lnk = debugdiv.find('.sqlpretty-hide-show');
           if (resdiv.is(':visible')) {
               lnk.html('Show ' + lnkname);
               resdiv.hide();
           } else {
               lnk.html('Hide ' + lnkname);
               resdiv.show();
           }
           return false;
       });
       jQuery(this).parent().css('visibility','visible');
   });
});
