<?hh // strict
namespace Decouple\ORM\Migration;
use Decouple\ORM\Migration\Migration;
use Decouple\ORM\Table\Create;
use Decouple\ORM\Table\Create\Field;
class MigrationManager {
  protected Vector<Migration> $migrations;
  public function __construct()
  {
    $this->migrations = Vector {};
  }
  public function addMigration(Migration $m) : void
  {
    $this->migrations->add($m);
  }
  public static function diff(Migration $a, Migration $b) : Map<string,mixed> 
  {
    $new = Map {};
    $changed = Map {};

    $aMap = $a->getMap();
    $bMap = $b->getMap();

    foreach($bMap as $name => $b_field) {
      if(!$aMap->contains($name)) {
        $new->set($name, $b_field);
      } else {
        $a_field = $aMap->get($name);
        if($b_field != null && $a_field != null) {
          $changed->set($name, static::fieldDiff($a_field, $b_field));
        }
      }
    }
    return Map {};
  }

  public static function fieldDiff(Field $a, Field $b) : (Map<string,mixed>,Map<string,mixed>)
  {
    $new = Map {};
    $changed = Map {};

    foreach($b->attributes as $attrib => $val) {
      if(!$a->contains($attrib)) {
        $new->set($attrib, $val);
      } else {
        if(serialize($a->attributes->at($attrib)) != serialize($b->attributes->at($attrib))) {
          $changed->set($attrib, $val);
        }
      }
    }
    return tuple($new, $changed);
  }
}
