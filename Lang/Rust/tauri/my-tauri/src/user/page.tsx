import { Link } from 'react-router-dom';
import { invoke } from "@tauri-apps/api/core";
import "../App.css";
import {use, useState , useEffect} from "react";

function ShowUsersTable( props){
    const list= props.list;
    return (
        <>
            <table className="table table-striped">
                <thead>
                <tr>
                    <th>Id</th><th>Name</th><th>Age</th>
                </tr>
                </thead>
                <tbody>
                {list.map((user) =>{
                    return (<tr key={user.id}><th>{user.id}</th><th>{user.name}</th><th>{user.age}</th></tr>) }
                )}
                </tbody>
            </table>
        </>
    )
}

export function SelectPage(){

    const [users,setUsers] = useState([])
    const [data, setData] = useState(Date.now());

    async function getUsers(): Promise<any[]> {
        let res = [];
        await invoke("select_table", {})
            .then(data => {
                data.forEach((row)  => { res.push(row); });
                setUsers(res);
            })
    }

    useEffect(() => {
        const interval = setInterval(() => {
            setData( Date.now())
            getUsers();
        }, 1000);

        return () => clearInterval(interval);    }, [])
    ;


    return(
        <>
            <div>Selected Users({data.toLocaleString()})</div>
            <ShowUsersTable list={users} />
            <button onClick={() => getUsers()}>Get Users</button>
        </>)
}

export function  AddListPage(){
    const [addUsers,setAddUsers] = useState([]);
    const [name,setName] = useState("");
    const [age, setAge] = useState<number>(10);

    async function insert() {
        await invoke("insert_table", {"insets" :
                addUsers.map(
                    (d) => { return { "name": d.name,"age": d.age} } ) }
        );
        setAddUsers([]);
        return;
    }

    function addList():void {
        let d  = { "id" : addUsers.length,"name": name,"age":age};
        setAddUsers([...addUsers,d])
    }

    return (
        <>
            <div>Add Users</div>
            <ShowUsersTable list={addUsers} />
            <form onSubmit={ (e) => {
                    e.preventDefault();
                    addList();
                }
                }>

                <input
                id="namePlace"
                placeholder={"Name"}
                onChange={( e)=>setName(e.target.value)
                }
                />
                <input
                type={"number"}
                placeholder={`${age}`}
                onChange={ (e)=>
                    setAge( e.currentTarget.valueAsNumber)
                }/>
                <br/>
                <button type="submit">Add User</button>
            </form>
            <br/>
            <button type={"button"} onClick={insert}>Register User</button>
        </>
    )
}

export default function HelloPage(){

    return (
        <>
            <SelectPage />
            <br/><br/>
            <AddListPage />
        </>
    )

}

