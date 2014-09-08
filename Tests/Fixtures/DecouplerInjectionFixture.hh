<?hh // partial
/** Mock classes used for DecouplerInjectionTest.hh **/

class MethodDependency {
  public function __construct(private int $number){}
    public function getNumber() {
      return $this->number;
    }
}

class UnregisteredDependency {}

class PropertyDependency {
  public function __construct(public int $value) {}
}

class ConstructorPropertyDependency {
  public function __construct(public int $value) {}
}


class MockMethod {
  public function getNumber(MethodDependency $dep) {
    return $dep->getNumber();
  }
  public function unregistered(UnregisteredDependency $dep) {
    return false;
  }
}

class MockInstance {
  public function __construct(public ConstructorPropertyDependency $param) {}
  public ?PropertyDependency $property = null;
}

class MockObject {
  public ?PropertyDependency $property;
  public function __construct(private int $value) {}
  public function value() : int {
    if($this->property instanceof PropertyDependency) {
      return $this->value + $this->property->value;
    }
    return $this->value;
  }
}
