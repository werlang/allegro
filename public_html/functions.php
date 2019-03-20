<?php
function printfile($file,$linenum=false) {
	ob_start();
	include($file);
	$contents = ob_get_contents();
	ob_end_clean();
	
	if ($linenum===false || $linenum===0)
		echo htmlentities($contents);
	else{
		$start=0;
		$end = strpos($contents, PHP_EOL);
		$length = $end-$start;
		$line = substr($contents,$start,$length);
		while($linenum>1){
			$start = $end+1;
			$end = strpos($contents, PHP_EOL,$start);
			$length = $end-$start;
			$line = substr($contents,$start,$length);
			$linenum--;
		}
		echo htmlentities($line);
	}
}
?>