<?hh // strict
namespace Decouple\Decoupler;
require_once dirname(__FILE__) . "/_Decouple.hh";
class Decoupler {
  public function __construct(protected Map<string,mixed> $dependencies) {
  
  }
  public function setAll(Map<string,mixed> $dependencies) : void {
    foreach($dependencies as $name => $dependency) {
      $this->set($name, $dependency);
    }
  }
  public function set(string $name, mixed $dependency) : void {
    $this->verifyName($name);
    $this->dependencies->set($name, $dependency);
  }
  public function contains(string $name) : bool {
    return $this->dependencies->contains($name);
  }
  /**
   * Get a specific dependency
   */
  public function get(string $name) : mixed {
    return $this->dependencies->get($name);
  }
  /**
   * Get all of the registered dependencies
   */
  public function getAll() : Map<string,mixed> {
    return $this->dependencies;
  }
  /**
   * Assert that the given class exists
   */
  protected function verifyName(string $name) : void {
    if(!class_exists($name)) {
      throw new \Exception(sprintf("%s is not a registered class name [Decoupler]", $name));
    }
  }
  /**
   * Inject an object
   */
  public function injectObject(mixed $object) : mixed {
    return decouple_inject_object($object, $this);
  }
  /**
   * Inject an instance
   */
  public function injectInstance(string $className) : mixed {
    return decouple_inject_instance($className, $this);
  }
  /**
   * Inject a method
   */
  public function injectMethod(mixed $object, string $method) : mixed {
    return decouple_inject_method($object, $method, $this);
  }
  /**
   * Inject a function
   */
  public function injectFunction(mixed $function) : mixed {
    return decouple_inject_function($function, $this);
  }
  /**
   * Inect a function, class (instance), or object
   */
  public function inject(mixed $target) : mixed {
    if(is_array($target) && count($target) == 2) {
      if(is_string($target[0])) {
        $object = $this->injectInstance($target[0]);
      } else if(is_object($target[0])) {
        $object = $this->injectObject($target[0]);
      } else {
        throw new \Exception(sprintf("Invalid class provided: %s", (string)$target[0]));
      }
      if(!method_exists($object, $target[1])) {
        throw new \Exception(sprintf("Invalid method requested: %s::%s", (string)$object, (string)$target[1]));
      }
      return $this->injectMethod($object, $target[1]);
    } else if(is_callable($target)) {
      return $this->injectFunction($target);
    } else if(is_object($target)) {
      return $this->injectObject($target);
    } else {
      throw new \Exception(sprintf("Unable to inject target %s", (string)$target));
    }
  }
}
