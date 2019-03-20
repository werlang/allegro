$(document).ready(function(){
	$(document).on( 'click', '.button-red.can-disable', function(){
		$(this).attr("disabled", true);
		$(this).prepend("<img src='icons/loading.gif' class='loadimg' />");
		//alert('clicou');
	} );
});

function enable_button(obj){
	obj.prop('disabled', false);
	obj.find('.loadimg').remove();
}