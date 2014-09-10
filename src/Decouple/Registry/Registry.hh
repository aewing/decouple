<?hh // strict
namespace Decouple\Registry;
class Registry {
  public Map<mixed,mixed> $entries;
  public function __construct(KeyedTraversable<mixed,mixed> $entries) {
    $this->entries = new Map($entries);
  }
  public function add(mixed $key, mixed $value) : void {
    $this->entries->set($key, $value);
  }
  public function get(mixed $key, mixed $default=false) : mixed {
    if($this->entries->contains($key)) {
      return $this->entries->at($key);
    }
    return $default;
  }
}
