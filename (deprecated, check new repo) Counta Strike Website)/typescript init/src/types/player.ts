// export type Player = {
//     id: number;
//     age: number;
//     name: string;
// }

// we use this as a sub-component to data
export type Content = {
	varType: string;
	varName: string;
	offset:  string | number; // technically a number
};

export type Data = {
	content: Content;
	size: number
}
