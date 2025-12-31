import asyncio

count = 0


async def hello_world():
    global count
    count += 1
    lcount = count
    print(f"[Sub  Thread {lcount}]Sleep 1sec")
    await asyncio.sleep(1)
    print(f"[Sub  Thread {lcount}]Hello World!")


async def main():
    # Schedule three calls *concurrently*:
    L = await asyncio.gather(
        hello_world(),
        hello_world(),
        hello_world(),
    )
    print(L)

print("[Main Thread] Start")
loop = asyncio.get_event_loop()
asyncio.run(main())

print("[Main Thread] End")

print("[Main Thread2] Start")
loop.create_task(hello_world())
print("[Main Thread2] End")
loop.run_until_complete(hello_world())
