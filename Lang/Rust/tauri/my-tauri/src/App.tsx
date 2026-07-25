import { useState } from "react";
import reactLogo from "./assets/react.svg";
import { invoke } from "@tauri-apps/api/core";
import "./App.css";

function App() {
  const [greetMsg, setGreetMsg] = useState("");
  const [name, setName] = useState("");

  //invoke("create_table");

  async function greet() {
    // Learn more about Tauri commands at https://tauri.app/develop/calling-rust/
    setGreetMsg(await invoke("greet", { name }));
  }

  const [pass, setPass] = useState("");
  const [msg, setMsg] = useState("");

  async function login() {
      await invoke('login', {'user': name.toString() , 'password' : pass.toString() })
          .then((message) => {setMsg(message);})
          .catch((error) => setMsg(error));

      await invoke('my_custom_command', {
          number: 42,
      })
          .then((res) =>
              console.log(`Message: ${res.message}, Other Val: ${res.other_val}`)
          )
          .catch((e) => console.error(e));
  }

  return (
    <main className="container">
      <h1>Welcome to Tauri + React</h1>

      <div className="row">
        <a href="https://vite.dev" target="_blank">
          <img src="/vite.svg" className="logo vite" alt="Vite logo" />
        </a>
        <a href="https://tauri.app" target="_blank">
          <img src="/tauri.svg" className="logo tauri" alt="Tauri logo" />
        </a>
        <a href="https://react.dev" target="_blank">
          <img src={reactLogo} className="logo react" alt="React logo" />
        </a>
      </div>
      <p>Click on the Tauri, Vite, and React logos to learn more.</p>

      <form
        className="row"
        onSubmit={(e) => {
          e.preventDefault();
          greet();
        }}
      >
        <input
          id="greet-input"
          onChange={(e) => setName(e.currentTarget.value)}
          placeholder="Enter a name..."
        />
        <button type="submit">Greet</button>
      </form>
        <p>{greetMsg}</p>
        <form onSubmit={ (e) => {
            e.preventDefault();
            login();
        }
        }>
            <input onChange={(e) => setName(e.currentTarget.value)}
                   placeholder="Enter a name..."/>
            <input onChange={(e) => setPass(e.currentTarget.value)}
                   placeholder="Enter a pass..."/>
            <button type={"submit"}
            >Login</button>
        </form>
        <p>{msg}</p>
    </main>
  );
}

export default App;
