<?hh // partial 
use Decouple\ORM\Connector;
define('ORMTestDir', dirname(__FILE__) . '/data');
class ConnectorTest extends PHPUnit_Framework_TestCase {
  private ?Connector $connector;
  public function testPdo() {
    if(!is_dir(ORMTestDir)) {
      if(!mkdir(ORMTestDir, true)) {
        throw new \Exception(sprintf("Invalid ORM test data directory: %s", ORMTestDir));
      }
    }
    /** Reset test data */
    $this->connector = null;
    $filename = sprintf("%s/test.sqlite", ORMTestDir);
    if(file_exists($filename)) {
      unlink($filename);
    }
    $connector = $this->_connector();
    $this->assertEquals($connector->connected(), true);
  }
  /*
  public function testTableCreate() {
    $connector = $this->_initConnector();
    $connector->table('articles')->create(function(Create $table) {
      $table->identifier('id');
      $table->string('name', 55);
      $table->text('content');
      $table->enum('status', ['draft','pending','published'])->default('draft');
      $table->timestamps();
    });
  }
   */
  public function _connector() : Connector {
    if(is_null($this->connector)) {
      $filename = sprintf("%s/test.sqlite", ORMTestDir);
      touch($filename);
      $this->connector = new Connector(sprintf("sqlite:%s", $filename), null, null);
    }
    return $this->connector;
  }
}
