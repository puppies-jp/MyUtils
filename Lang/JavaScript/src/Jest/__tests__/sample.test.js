test('test 1.', () => {
  expect(1 + 2).toBe(3);
});


function async_func() {
  return new Promise(resolve => {
    console.log('start async_func');
    setTimeout(function () {
      console.log('end async_func');
      resolve('done!');
    }, 100);
  });
}

function async_func_err() {
  return new Promise((resolve, reject) => {
    console.log('start async_func_err');
    setTimeout(function () {
      console.log('end async_func_err');
      reject('error!');
    }, 100);
  });
}
test('async promiss resolve test 1', () => {
  return async_func().then(msg => {
    console.log('resolve test');
    expect(msg).toBe('done!');
  });
});

test('async promiss resolve test 2', () => {
  return expect(async_func()).resolves.toBe('done!');
});

test('async promiss reject test', () => {
  return async_func_err().catch(msg => {
    console.log('reject test');
    expect(msg).toBe('error!');
  });
});

test('async promiss reject test 2', () => {
  return expect(async_func_err()).rejects.toBe('error!');
});

test('await ', async () => {
  const ret = await async_func();
  console.log(ret);
  expect(ret).toBe('done!');
});

test('await 2', async () => {
  await expect(async_func()).resolves.toBe('done!');
});
