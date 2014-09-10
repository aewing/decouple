<?hh // partial 
require_once dirname(__FILE__) . "/Xhp/Base.hh";
/**
 * Load a single Decouple UI XHP Definition
 */
function decouple_load_xhp(string $name) : void {
  $filename = (string)dirname(__FILE__) . '/Xhp/Elements/' . (string)ucfirst($name) . '.hh';
  if(!file_exists($filename)) {
    throw new \Exception(sprintf("Could not locate XHP asset %s [%s]", $name, $filename));
  }
  require_once($filename);
}
/**
 * Load all Decouple UI XHP Definitions in a given directory
 */
function decouple_load_xhp_dir(string $name, bool $require=true) {
  $dir = decouple_xhp_dir() . $name;
  if(!is_dir($dir)) {
    throw new Exception(sprintf("Invalid XHP directory requested: %s", $dir));
  }
  if($require) {
    foreach(glob($dir . '/*.hh') as $filename) {
      require_once $filename;
    }
    return;
  }
  set_include_path(get_include_path() . PATH_SEPARATOR . $dir);
  spl_autoload_register();
}
/**
 * Retrieve the Decouple UI XHP directory
 */
function decouple_xhp_dir() {
  return (string)dirname(__FILE__) . '/Xhp/';
}
