<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5 - Caixa de texto</title>

<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script src="script/popups.js"></script>
<script src="script/buttons.js"></script>
<link rel="stylesheet" href="css/stylesheet.css"/>
<link rel="stylesheet" href="css/popups.css"/>
<link rel='stylesheet' href='css/responsive.css'/> 
<link rel='stylesheet' href='css/buttons.css'/> 

</head>
<body>
<?php include_once("functions.php"); ?>
<?php include_once("side_menu.php"); ?>

<div class='page-wrapper'>
	<div class="container">
		<div class='col-m-12'>
			<h1>Mostrando caixas de texto</h1>
			
			<p>Podemos usar o allegro para mostrar caixas de texto nativas da interface gráfica do sistema operacional. Esta função é útil quando queremos reportar algum erro no allegro, como por exemplo falha ao encontrar arquivos de imagens, sons, etc. Vamos ao código:</p>
			
			<pre class="prettyprint linenums" id='codigo'><?php printfile('ccode/mensagem.c'); ?></pre>
		
			<p>Na <span class='linha' data-ln='2'>linha 2</span> fazemos o include do módulo allegro_native_dialog, que permite a chamada da janela.</p>
			
			<p>Nas <span class='linha' data-ln='7'>linha 7</span>, <span class='linha' data-ln='9'>linha 9</span> e <span class='linha' data-ln='11'>linha 11</span> declaramos strings que contém o título da caixa de mensagem, o título do texto que fica dentro da caixa, e o próprio texto da caixa.</p>
			
			<h2><a target='_blank' href='https://www.allegro.cc/manual/5/al_show_native_message_box'>al_show_native_message_box()</a></h2>
			<p>Na <span class='linha' data-ln='13'>linha 13</span> chamamos a função al_show_native_message_box(). Esta função recebe 6 parâmetros, definidos a seguir:</p>

			<ul>
				<li>O display sobre o qual a caixa será mostrada, ou NULL.</li>
				<li>A string contendo o título da caixa.</li>
				<li>A string contendo o título da mensagem da caixa.</li>
				<li>A string contendo a mensagem da caixa.</li>
				<li>O texto dos botões que serão mostrados. No Windows 10, este parâmetro não está funcionando. Informamos NULL por padrão.</li>
				<li>A flag que indica o tipo de caixa de texto que será mostrada. Os valores possíveis são os seguintes:</li>
				<ul>
					<li>ALLEGRO_MESSAGEBOX_WARN - Mensagem de aviso com botão OK</li>
					<li>ALLEGRO_MESSAGEBOX_ERROR - Mensagem de erro com botão OK</li>
					<li>ALLEGRO_MESSAGEBOX_QUESTION - Pergunta com botão OK</li>
					<li>ALLEGRO_MESSAGEBOX_OK_CANCEL - Informação com botões OK e Cancelar</li>
					<li>ALLEGRO_MESSAGEBOX_YES_NO - Informação com botões Sim e Não</li>
				</ul>
			</ul>

			<p>A função retorna 0 caso o botão pressionado por OK/Sim, ou 1 caso seja Cancelar/Não.</p>
			
			<p>Uma observação importante a ser feita sobre a função é que ela não requer que a biblioteca allegro tenha sido inicializada (com <a target='_blank' href='https://www.allegro.cc/manual/5/al_init'>al_init()</a>), portanto esta caixa de texto pode ser usada para reportar erros de inicialização dos diferentes módulos do allegro. Nos exemplos a seguir, usaremos os valores de retorno das funções de inicialização do allegro para testar se houve erro ou não.</p>

			<div class='imagem'>
				<img src='img/native_box.png' >
				<label>Caixa de mensagem nativa do Windows 10</label>
			</div>

			<h3>Outros links</h3>
			<ul>
				<li><a target='_blank' href='http://www.rafaeltoledo.net/tutorial-allegro-5-15-utilizando-caixas-de-dialogo-nativas/'>Utilizando Caixas de Diálogo Nativas</a></li>
				<li><a target='_blank' href='https://wiki.allegro.cc/index.php?title=Showing_a_basic_native_GUI_message_box'>Showing a basic native GUI message box</a></li>
			</ul>
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='imagem.php'>Anterior</a>
			<a class="button-red" href='fontes.php'>Próximo</a>
		</div>
	</div>
	
</div>
</body>
<script src="script/line_link.js"></script>
