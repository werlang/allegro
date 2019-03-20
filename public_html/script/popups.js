function summon_tooltip(obj,pos,text){
	$('.tooltip').fadeOut(500);
	$('.tooltip.append').remove();
	var objname = $(obj).attr("name");
	if (!text){
		if ($(".tooltip[data-tooltipfor='"+objname+"']").length ) {
			var tooltipobj = $(".tooltip[data-tooltipfor='"+objname+"']");
			if ($(".tooltip.up[data-tooltipfor='"+objname+"']").length){
				var left = obj.position().left + obj.outerWidth() / 2 - tooltipobj.outerWidth() / 2;
				var top = obj.position().top - tooltipobj.outerHeight() -5;
			}
			if ($(".tooltip.down[data-tooltipfor='"+objname+"']").length){
				var left = obj.position().left + obj.outerWidth() / 2 - tooltipobj.outerWidth() / 2;
				var top = obj.position().top + obj.outerHeight() +5;
			}
			if ($(".tooltip.left[data-tooltipfor='"+objname+"']").length){
				var top = obj.position().top + obj.outerHeight() / 2 - tooltipobj.outerHeight() / 2;
				var left = obj.position().left - tooltipobj.outerWidth() -5;
			}
			if ($(".tooltip.right[data-tooltipfor='"+objname+"']").length){
				var top = obj.position().top + obj.outerHeight() / 2 - tooltipobj.outerHeight() / 2;
				var left = obj.position().left + obj.outerWidth() +5;
			}
			tooltipobj.css({'left':left+'px', 'top':top+'px'});
			tooltipobj.show();
		}
	}
	else{
		text = text.replace(/>/g,'&gt;').replace(/</g,'&lt;');
		obj.parent().append("<div class='tooltip append'>"+text+"</div>");
		var tooltipobj = $(".tooltip.append");

		if (pos == 'up')
				var left = obj.position().left + obj.outerWidth() / 2 - tooltipobj.outerWidth() / 2;
		if (pos == 'down')
				var left = obj.position().left + obj.outerWidth() / 2 - tooltipobj.outerWidth() / 2;
		if (pos == 'left')
				var left = obj.position().left - tooltipobj.outerWidth() -5;
		if (pos == 'right')
				var left = obj.position().left + obj.outerWidth() +5;
		
		//passou limite esquerdo
		if (left < 0)
			left = 0;

		//passou limite direito
		var right = left + tooltipobj.outerWidth();
		if (right >= $(window).width() ){
			left -= right - $(window).width();
		}

		//voltou a passar o esquerdo
		if (left < 0){
			tooltipobj.width(tooltipobj.width()+left);
			left = 0;
		}
		
		if (pos == 'up')
				var top = obj.position().top - tooltipobj.outerHeight() -5;
		if (pos == 'down')
				var top = obj.position().top + obj.outerHeight() +5;
		if (pos == 'left')
				var top = obj.position().top + obj.outerHeight() / 2 - tooltipobj.outerHeight() / 2;
		if (pos == 'right')
				var top = obj.position().top + obj.outerHeight() / 2 - tooltipobj.outerHeight() / 2;

		tooltipobj.css({'left':left+'px', 'top':top+'px'});
		//alert(left+' '+top);
		tooltipobj.show();
	}
}

function create_popup(content,divclass,dimension,url){
	$('.container').append("<div class='popup'><div class='"+divclass+"'></div></div>");
	$('.popup').click(function(e){ 
		if ( e.target == this || divclass == "message" )
			$('.popup').remove();
	});
	if (divclass == 'window'){
		$('.popup .window').css({width: dimension});
		if ($('.popup').width() < dimension)
			$('.popup .window').addClass('m-size');
		else
			$('.popup .window').addClass('f-size');
	}
	$('.popup .'+divclass).html(content);
	$('.popup').show();
	if (url){
		$('.popup').click( function() {
			location.href = url;
		});
	}
}

function show_message(text,url){
	create_popup(text,"message",null,url);
}

$(document).ready( function() {
	//$(window).resize( function() { $('.popup').click(); });
});