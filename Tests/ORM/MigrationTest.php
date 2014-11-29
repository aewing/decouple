<?hh // partial 
use Decouple\ORM\Migration\Migration;
use Decouple\ORM\Migration\MigrationManager;
use Decouple\ORM\Connector;
class MigrationTest extends PHPUnit_Framework_TestCase {
  protected ?Connector $connector;
  private string $ORMTestDir = 'Tests/ORM/data';
  public function testMigrationA() {
    $connector = $this->_connector();
    $table = new Migration('articles', $connector->driver());
    $table->identifier('id');
    $table->string('title', 55);
    $table->text('content')->nullable()->default(null);
    $table->enum('status', ['draft','pending','published'])->default('draft');
    $table->timestamps();

    $serialized = $table->toString();
    $duplicated = Migration::restore('articles', $serialized, $connector->driver());
    $duplicated->text('diff')->nullable()->default(null);

    $diff = MigrationManager::diff($table, $duplicated);

    var_dump($diff);
    exit();

    $manager = new MigrationManager();
    $manager->addMigration($table);
  }
  public function _connector() : Connector {
    if(is_null($this->connector)) {
      $filename = sprintf("%s/test.sqlite", $this->ORMTestDir);
      $this->connector = new Connector(sprintf("sqlite:%s", $filename), null, null);
    }
    return $this->connector;
  }
}
