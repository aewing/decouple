<?hh // partial 
namespace Decouple\Ui;
require_once dirname(__FILE__) . "/_Ui.hh";
/**
 * Decouple UI Class
 * Used to load and render Decouple UI XHP Elements
 * 
 * @author Andrew Ewing
 * @package Decouple
 * @subpackage Ui
 */
class Ui {
  /**
   * Create a Decouple UI XHP element
   * @param string $name - The name of the XHP element to create
   * return :xhp - A new XHP element
   */
  public static function element(string $name) : :xhp {
    decouple_load_xhp($name);
    $className = 'xhp_' . str_replace(':', '__', $name);
    if(class_exists($className)) {
      $obj = new $className([],[]);
      return $obj;
    }
    throw new \Exception(sprintf("Invalid UI element requested: %s", $name));
  }
  /**
   * Autoload all Decouple UI XHP Elements
   */
  public static function autoload() : void {
    decouple_load_xhp_dir('Elements');
  }
}
