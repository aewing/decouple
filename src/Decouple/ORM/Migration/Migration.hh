<?hh // strict
namespace Decouple\ORM\Migration;
use Decouple\ORM\Table\Create;
use Decouple\ORM\Connector;
use Decouple\ORM\Create\Field;
class Migration extends Create {
  public static function restore(string $name, string $string, Connector $connector) : Migration
  {
    $migration = new Migration($name, $connector);
    $migration->fromString($string);
    return $migration;
  }
  public function getMap() : Map<string,Field> {
    return $this->map;
  }
}
