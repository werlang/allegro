$(document).ready(function () {
	//disable nos menus
	if ($('#disable-menu').html()=='disciplinas'){
		$('.side-menu-list li').not("#profile, #sair, #disciplinas").css({"pointer-events":"none","opacity":".3"});
	}
	else if ($('#disable-menu').html()=='questoes'){
		$('.side-menu-list li').not("#profile, #sair, #disciplinas, #inserir").css({"pointer-events":"none","opacity":".3"});
	}

	//some tudo se clica na fog
	$('.fog').click( function() {
		$('.fog').hide();
		if ( $(window).width() < 1200 ){
			$('.header-toggle').show();
			$('.side-menu').hide();
		}
	});

	//clicou no botao mobile que mostra o header
	$('.header-toggle').click( function(){
		$(this).css({opacity: '1', backgroundColor: '#888', backgroundImage: 'url(#)'});
		$(this).animate({width: '260px', height: '100%'}, 200, function() {
			$(this).removeAttr('style');
			$(this).hide();
			$('.side-menu').show();
			$('.fog').fadeIn(100);
		});
	} );
	
	$(window).resize( function () {
		if ( $(window).width() >= 1200 ){
			$('.side-menu').show();
			$('.fog').click();
		}
		else{
			$('.side-menu').hide();
		}
	});
	
});

