if (typeof SQLPRETTY == "undefined") {
    var SQLPRETTY = {};
}	
jQuery(document).ready(function() {
    SQLPRETTY.init = function() {
        var sqlid = '';
        if (arguments[0]) {
            sqlid = arguments[0];
            sqlid.replace('#','');
            sqlid = '#' + sqlid + ' ';
        }
        jQuery(sqlid + '.sqlpretty-result').hide();
        
        jQuery(sqlid + '.sqlpretty-hide-show').each(function(idx) {
           var lnkname = jQuery(this).html()
           jQuery(this).html('Show ' + lnkname);
           jQuery(this).unbind('click.sqlpretty');
           jQuery(this).bind('click.sqlpretty',function() {
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
    };
    SQLPRETTY.init();
});
