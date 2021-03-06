<?php
/**
 *	@file: Render.inc
*	@author: Soyoes 2014/01/10
*	@uses: html template render class
*	@example:
*****************************************************************************/


/**
 * TODO solve <script> <style> without {ignore}
 * 
 * TODO add custome tags like <liber::pages>
 * 
 * 
 */
class Render {
	private static $var_prefix = 'LBR_';

	private static $output_path;
	private static $ext = '.html';
	
	private $layout = '_layout';
	private $data = [];
	private $contents = [];
	private $path;
	
	private function __construct(){}
	
	static function factory($path){
		self::$output_path = APP_DIR.__SLASH__.'tmp'.__SLASH__;
		$render = new Render();
		$render->path = $path.__SLASH__;
		return $render;
	}
	
	function assign($key, $value){
		$this->data[$key] = $value;	
	}
	
	/**
	 * print to screen
	 * 
	 */
	function render($file,$data=[],$layout=null,$renderOnly=false){
		$req = REQ::getInstance();
		$template = isset($layout)? $layout : $this->layout;
		$ns = $req?$req->getNamespace():"";
		$ns = empty($ns)?"":$ns."-";
		$key = 'template-'. REQ::getTemplateType()."-".$ns.$template;		
		$wrapper_code = cache_get($key, function($f){
			$req = REQ::getInstance();
			$ns = $req?$req->getNamespace():"";
			$ns = empty($ns)?"":$ns."-";
			$key_prefix = 'template-'.REQ::getTemplateType()."-".$ns;
			$fn = $this->path.str_replace($key_prefix,'',$f);
			if(!str_ends($fn,'.html')) $fn.='.html';	
			return file_get_contents($fn);
		},false);
				if(!empty($data))
		foreach ($data as $k=>$v)
			$this->data[$k] = $v;
		$this->data['__render'] = $this;
		$req = REQ::getInstance();
		if($req){
			if(!$this->data['__controller']) $this->data['__controller'] = $req->getController();
			if(!$this->data['__namespace']) $this->data['__namespace'] = $req->getNamespace();
			if(!$this->data['__action']) $this->data['__action'] = $req->getAction();
			if(!$this->data['__params']) $this->data['__params'] = $req->params;
		}

		$_REQUEST[self::$var_prefix."TMP_DATA"] = $this->data;
		extract($this->data, EXTR_PREFIX_ALL, self::$var_prefix);
		$r = $this->render_file($file,$wrapper_code);
		$output = null;
		if($r){
			if($renderOnly){
				ob_start(); 
				include($r);
				$output = ob_get_contents();
				// REQ::write($output, "html");
				ob_end_clean();
				// flush();
			}else{
				include($r);
			}
		}
		unset($this->data);
		unset($data);
		if($output)
			return $output;
	}
	
	/**
	 * get phpcode from file
	 * 
	 */
	function render_file($file,$template_code){
		$prefix  = 'template-' . REQ::getTemplateType() . '-';
		if(!empty($this->data['__namespace']))
			$prefix .= $this->data['__namespace']."-";
		$filepath = $this->path.$file;
		if(!file_exists($filepath)) return false;
		$outpath = self::$output_path. $prefix .str_replace(self::$ext,'.php',str_replace('/','--',$file));
		if(!file_exists($outpath)
			//||(Consts::$mode=="Developing" && filemtime($filepath) > filemtime($outpath))){
			||Consts::$mode=="Developing"){
			$code = $this->compile($filepath,$template_code);
			if(isset($code) && $code!=""){
				file_put_contents($outpath,$code);
				unset($code);
			}
		}
		return $outpath;
	}

	function compile($file,$wrapper){
		list($before, $after) = $wrapper?explode('__CONTENTS__', $wrapper):["",""];
		$src = $before.file_get_contents($file).$after;
		$rows = preg_split('/(\{[^\{^\}]*\})/', $src, -1, PREG_SPLIT_DELIM_CAPTURE | PREG_SPLIT_NO_EMPTY ) ;
		$phpcode = '';
		$indent = 0;
		$ignore = false;
		//CustomTag get user tags from delegate
		$delegate_methods = get_class_methods('RenderDelegate');
		$custom_tags = [];
		foreach ($delegate_methods as $m) 
			if(str_starts($m,'tag_'))
				$custom_tags []= preg_replace('/^tag_/','',$m);
		
		//CustomTag make regex compression
		$tags_regexp = (!empty($custom_tags)) ?
			'(%|=|if|elseif|else|break|ignore|for|var|include|'.join('|',$custom_tags).')':
			'(%|=|if|elseif|else|break|ignore|for|var|include)';
		
		while($code = array_shift($rows)){
			$matched = false;
			preg_match_all('/\{(?P<close>\/*)(?P<tag>'.$tags_regexp.'{0,1})\s*(?P<val>.*)\}/', $code, $matches);
			
			if(empty($matches[0])){
				$phpcode .= $code;
			}else{
				list($close, $tag, $val) =  [$matches['close'][0]=="/"?true:false, $matches['tag'][0], trim($matches['val'][0])];
				if($tag=='' || $tag=='=')$tag='echo';
				if($tag=='%')$tag='text';
				$val = $tag=="text"?$val: preg_replace('/\.([a-zA-Z0-9_]+)/', "['$1']",$val);
				if(!preg_match('/\$(_GET|_POST|_REQUEST|_SESSION)/', $val))
					$val = preg_replace('/\$/','$'.self::$var_prefix."_",$val);
				if($close){
					if($tag=='if'||$tag=='for')$indent --;
					if($tag=='ignore'){
						$ignore = false;
					}else{
						$phpcode .= '<?php } ?>';
					}
				}else if($ignore){
					$phpcode .= $code;
				}else if(!empty($custom_tags)&&in_array($tag, $custom_tags)){
					//custom tags
					$phpcode .= "<?php echo RenderDelegate::tag_{$tag}(".(empty($val)?'""':'"'.$val.'"').", \$_REQUEST['".self::$var_prefix."TMP_DATA']); ?>";
				}else{
					switch($tag){
						case 'for':
							$parts = preg_split('/\s*,\s*/',$val,-1,PREG_SPLIT_NO_EMPTY );
							$len = count($parts);
							$indent ++;
							switch($len){
								case 1:$phpcode .= '<?php foreach('.$parts[0]." as $".self::$var_prefix."_key=>$".self::$var_prefix."_value) { ?>";break;
								case 2:$phpcode .= '<?php foreach('.$parts[0]." as $".self::$var_prefix."_key=>".$parts[1].") { ?>";break;
								default :
									if((preg_match('/^\d+$/', $parts[1])) || (preg_match('/^\$/', $parts[1])) && (preg_match('/^\d+$/', $parts[2]))|| (preg_match('/^\$/', $parts[2]))){
										$phpcode .= '<?php for($'.$parts[0].'='.$parts[1].';$'.$parts[0].'<'.$parts[2].';$'.$parts[0].'++) { ?>';
									}else
										$phpcode .= '<?php foreach('.$parts[0].' as '.$parts[1].'=>'.$parts[2].') { ?>';break;
							}
							break;
						case 'if':
							$indent ++;
							$phpcode .= '<?php if('.$val.'){ ?>';break;
						case 'elseif':
							$phpcode .= '<?php }else if('.$val.'){ ?>';break;
						case 'else':
							$phpcode .= '<?php }else{ ?>';break;
						case 'break':
							$phpcode .= '<?php break; ?>';break;
						case 'echo':
							$phpcode .= '<?= '.$val.' ?>';break;
						case 'text':
							$vstr = preg_split('/,+/', trim($val));
							if(count($vstr)>1){
								$vstr = array_map(function($e){return trim($e);}, $vstr);
								$phpcode .= '<?= T("'.join('","',$vstr).'"); ?>';break;
							}else
								$phpcode .= '<?= T("'.$val.'"); ?>';break;
						case 'var':
							$phpcode .= '<?php '.$val.'; ?>';break;
						case 'include':
							$phpcode .= '<?php $__render->include_template("'.preg_replace('/\'"/',"",$val).'"); ?>';break;
						case 'ignore':
							$ignore = true;
							break;
						default:
							break;
					}//end switch
				}//end else
			}
		}
		return $phpcode;
	}

	function include_template($f){
		$r = $this->render_file($f.'.html');
		$output = '';
		if($r) {
			ob_start(); include($r);
			$output = ob_get_contents();
			ob_end_clean();
		};
		echo $output;
		flush();
	}
	
	/**
	 * 
	 * @param page: current page number
	 * @param total: total records
	 * @param opts: assoc-array (paginate options), perPage|items
	 * @return : array of page index numbers. (starts with 1)
	 * @example : 
	 * 		Render::paginate(11,1000);
	 		// result :{pages:[1,0,7,8,9,10,11,12,13,14,15,0,100], cursor:6}
	 		// 0 means nothing/blank/...
	 * */
	static function paginate($page,$total,$opts=['perPage'=>20]){
		$pp = ($opts['perPage']>0)? $opts['perPage']: 20;//perpage
		$pi = $opts['items']?$opts['items']:9;//page button amount
		$ptotal = ceil($total/$pp);
		$size 	= min($ptotal, max(7,$pi));
		$pages 	= [$page];
		if($ptotal>$size){
			$seg = $size%2==0?$size+1:$size;
			for ($i=1;count($pages)<$seg;$i++){
				if($page-$i>=1) array_unshift($pages,$page-$i);
				if($page+$i<=$ptotal)$pages[]=$page+$i;
			}
			if(end($pages)<=$ptotal-1)
				$pages=array_merge(array_slice($pages,0,count($pages)-2),[0,$ptotal]);
			if($pages[0]>=2)
				$pages=array_merge([1,0],array_slice($pages,2,count($pages)));
		}else{
			$pages = [];
			for($i=1;$i<=$ptotal;$i++)
				if(!in_array($i,$pages))
					$pages[]=$i;
		}
		return ['pages'=>$pages,'cursor'=>array_search($page,$pages)];
	}
}

