<?hh // strict
namespace Decouple\ORM\Migration;
use Decouple\ORM\Table\Create;
use Decouple\ORM\Connector;
use Decouple\ORM\Table\Create\Field;
class Migration extends Create {
  public static function restore(public string $name, string $string, string $connector_type='mysql') : Migration
  {
    $migration = new Migration($name, $connector_type);
    $migration->fromString($string);
    return $migration;
  }
  public function getMap() : Map<string,Field> {
    return $this->map;
  }
}
