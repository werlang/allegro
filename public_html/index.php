<!DOCTYPE html>
<head>
<meta charset='utf-8' />
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<title>Aprendendo Allegro 5</title>

<script src="https://cdn.rawgit.com/google/code-prettify/master/loader/run_prettify.js"></script>
<script src="https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js"></script>
<script src="script/popups.js"></script>
<script src="script/buttons.js"></script>
<link rel="stylesheet" href="css/stylesheet.css"/>
<link rel="stylesheet" href="css/popups.css"/>
<link rel='stylesheet' href='css/responsive.css'/> 
<link rel='stylesheet' href='css/buttons.css'/> 
<link rel='stylesheet' href='css/index.css'/> 

</head>
<body>
<?php include_once("functions.php"); ?>
<?php include_once("side_menu.php"); ?>

<div class='page-wrapper'>
	<div class="container">
		<div id='img-head' class=''></div>
		<div class='col-m-12'>
			<h1>Sobre mim</h1>
			
			<p>Sou o prof. Pablo Werlang, dou aula no IFSul desde 2011, e desenvolvi estes tutoriais para ajudar meus alunos com uma demanda que sempre senti neles: o desenvolvimento de software na linguagem C que utilizem uma interface gráfica.</p>
			
			<p>Como dou aula principalmente em disciplinas que utilizam linguagem C, resolvi apresentar para meus alunos a biblioteca Allegro.</p>
			
			<p>Qualquer dúvida, só me enviar um <a target='_blank'  href="mailto:pswerlang@gmail.com">email</a>, ou entrar em contato comigo via <a target='_blank'  href="http://www.facebook.com/pswerlang">facebook</a>.</p>

			<h1>Sobre o Allegro</h1>
			
			<p>Retirado e traduzido do site <a target='_blank' href="http://liballeg.org/">liballeg.org</a></p>

			<p class='quote'>Allegro é uma biblioteca multi-plataforma direcionada principalmente para o desenvolvimento de jogos e programação multimídia. Ela lida com tarefas de baixo nível como criar janelas, entrada do usuário, carregar dados, desenhar imagens, reproduzir sons, etc.</p>

			<p>Esta série de tutoriais foi desenvolvido usando a versão 5.0.10 do Allegro e seu conteúdo foi fortemente inspirado nos tutoriais encontrados <a target='_blank'  href="http://www.rafaeltoledo.net/tutoriais-allegro-5/">nesta página</a>. Recomendo dar uma olhada lá.</p>
			
			<p>Uma outra ótima fonte de informações sobre a biblioteca, é a <a target='_blank' href='https://wiki.allegro.cc/'>wiki.allegro.cc</a>, lá você acha de tudo! Destaque para os <a target='_blank' href='https://wiki.allegro.cc/index.php?title=Allegro_5_API_Tutorials'>tutoriais sobre a biblioteca</a>.

			<p>A página <a target='_blank'  href="https://www.allegro.cc/">allegro.cc</a> possui uma série de recursos sobre a biblioteca, como forum, projetos, e principalmente o <a target='_blank'  href="https://www.allegro.cc/manual/5/index.html">manual</a>. Sempre que quiser saber sobre uma função, consulte o manual, além de outras referências pela internet afora, como <a target='_blank'  href="https://www.youtube.com/playlist?list=PL9333715188CD7669">esta excelente web serie</a> explicando todos os passos que levam a criação de jogos 2D usando o allegro 5.</p>

			<p>Os tutoriais a seguir levam em consideração que você já está com a biblioteca funcionando em seu ambiente de desenvolvimento. Caso não seja o caso, recomendo que dê uma olhada <a target='_blank'  href="allegro_install.pdf">neste outro tutorial</a>, ensinando como deixar o allegro funcionando no code::blocks.</p>
			
			
		</div>
		<div id='btn-footer'>
			<a class="button-red" href='janela.php'>Começar</a>
		</div>
	</div>
</div>
</body>
<script src="script/line_link.js"></script>
