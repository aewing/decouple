<?hh // partial
function decouple_inject_function(mixed $function, Decouple\Decoupler\Decoupler $decoupler) : mixed {
  // Create reflection object method
  $rf = new ReflectionFunction($function);
  $params = decouple_function_params($rf, $decoupler);
  return $rf->invokeArgs($params);
}

function decouple_inject_method(mixed $object, string $method, Decouple\Decoupler\Decoupler $decoupler) : mixed {
  // Create reflection object method
  $rm = new ReflectionMethod($object, $method);
  $params = decouple_method_params($rm, $decoupler);
  return $rm->invokeArgs($object, $params);
}

function decouple_inject_instance(string $className, Decouple\Decoupler\Decoupler $decoupler) : mixed {
  if(!class_exists($className)) {
    throw new Exception(sprintf("%s is not a valid class name [Decoupler]", $className));
  }
  // Reflect on the object
  $rfo = new ReflectionClass($className);
  // Reflect on the constructor
  $rfm = $rfo->getConstructor();
  if(!is_null($rfm)) {
    $params = decouple_method_params($rfm, $decoupler);
  } else {
    $params = [];
  }
  $obj = $rfo->newInstanceArgs($params);
  return decouple_inject_object($obj, $decoupler);
}

function decouple_inject_object(mixed $obj, Decouple\Decoupler\Decoupler $decoupler) : mixed {
  $rfo = new ReflectionObject($obj);
  // Reflect on the properties
  $rfp = $rfo->getProperties();
  foreach($rfp as $property) {
    /*
    if(isset($property->info)) {
      if(substr($property->info['type'], 0, 1) == '?') {
        $type = str_replace('?', '', $property->info['type']));
        if (class_exists($type)) {
          if($decoupler->contains($type)) {
            $name = $property->info['name'];
            $obj->$name = $decoupler->get($type);
          }
        }
      }
    }
    */
    var_dump($property);exit();
  }
  return $obj;
}

function decouple_method_params(ReflectionMethod $rm, Decouple\Decoupler\Decoupler $decoupler) {
  $rp = $rm->getParameters();
  $params = [];
  foreach($rp as $param) {
    $param = (array)$param;
    if(!$decoupler->contains($param['info']['type'])) {
      throw new Exception(sprintf("Unregistered dependency: %s [Decoupler]", $param['info']['type']));
    } else {
      $params[] = $decoupler->get($param['info']['type']);
    }
  }
  return $params;
}

function decouple_function_params(ReflectionFunction $rf, Decouple\Decoupler\Decoupler $decoupler) {
  $rp = $rf->getParameters();
  $params = [];
  foreach($rp as $param) {
    $param = (array)$param;
    if(!$decoupler->contains($param['info']['type'])) {
      throw new Exception(sprintf("Unregistered dependency: %s [Decoupler]", $param['info']['type']));
    } else {
      $params[] = $decoupler->get($param['info']['type']);
    }
  }
  return $params;
}
