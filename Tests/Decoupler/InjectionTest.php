<?hh // partial 
require_once dirname(__FILE__) . '/../Fixtures/DecouplerInjectionFixture.php';
/**
 * Test the Decouple dependency injector
 */
class DecouplerInjectionTest extends PHPUnit_Framework_TestCase {
  /**
   * Test object method injection
   */
  public function testObjectMethodInjection() : void {
    // Create a mock method
    $mock = new MockMethod();
    // Method dependency stores a private int
    $decoupler = new Decouple\Decoupler\Decoupler(Map::fromArray([
      'MethodDependency' => new MethodDependency(42)
    ]));
    // Get the private int 
    $result = $decoupler->injectMethod($mock, 'getNumber');
    // Expect 42
    $this->assertEquals($result, 42);
  }

  /**
   * Test object method injection failure & error messages
   */
  public function testObjectMethodInjectionFailure() : void {
    // Create a mock method
    $mock = new MockMethod();

    // Method dependency stores a private int
    $decoupler = new Decouple\Decoupler\Decoupler(Map::fromArray([
      'MethodDependency' => new MethodDependency(1)
    ]));

    // Force an unregistered dependency error
    $error = '(none)';
    try {
      $decoupler->injectMethod($mock, 'unregistered');
    } catch(Exception $e) {
      $error = $e->getMessage();
    }

    // Make sure the error message checks out
    $this->assertEquals($error, 'Unregistered dependency: UnregisteredDependency [Decoupler]');
  }

  /**
   * Test instance creation and property injection
   */
  public function testInstanceInjection() : void {
    // PropertyDependency & ConstructorPropertyDependency have public int $value as constructor args
    $decoupler = new Decouple\Decoupler\Decoupler(Map::fromArray([
      'PropertyDependency' => new PropertyDependency(42),
      'ConstructorPropertyDependency' => new ConstructorPropertyDependency(42)
    ]));

    // Create a mock instance
    $mock = $decoupler->injectInstance('MockInstance');
    // Default propery & value
    $property = null;
    $value = null;
    // Make sure we're getting the right type of object back
    $this->assertEquals(($mock instanceof MockInstance), true);
    if ($mock instanceof MockInstance) {
      // Make sure our property was injected
      $property = $mock->property;
      $this->assertEquals(($property instanceof PropertyDependency), true);
      if($property instanceof PropertyDependency) {
        // Make sure our property value is accessible
        $value = $property->value;
        $this->assertEquals($value, 42);
        // Make sure our constructor parameter was injected
        $param = $mock->param;
        $this->assertEquals(($param instanceof ConstructorPropertyDependency), true);
        if($param instanceof ConstructorPropertyDependency) {
          // Make sure our constructor parameter value is accessible
          $pvalue = $param->value;
          $this->assertEquals($pvalue, 42);
        }
      }
    }
  }

  /**
   * Test pre-existing object property injection
   */
  public function testObjectInjection() : void {
    // Property dependency has public int $value
    $decoupler = new Decouple\Decoupler\Decoupler(Map::fromArray([
      'PropertyDependency' => new PropertyDependency(21)
    ]));
    // Create a mock object
    $object = new MockObject(21);
    // Inject properties
    $decoupler->injectObject($object);
    // Make sure our properties were injected properly
    $property = $object->property;
    if($property instanceof PropertyDependency) {
      // Make sure our property value is accessible
      $value = $property->value;
      $this->assertEquals($value, 21);
      // Make sure our object methods are functioning and can access property
      $this->assertEquals($object->value(), 42);
    }
  }
}
