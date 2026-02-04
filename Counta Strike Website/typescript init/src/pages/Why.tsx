import type { ReactNode, CSSProperties } from "react";
import React, { useState } from "react";
import DisplayOffsets from "../components/CoolOffsetText/DisplayOffsets.tsx";
import clientJson from "../../data/client_dll.json";
import serverJson from "../../data/server_dll.json";
import offsetsJson from "../../data/offsets.json";
import Tab from "react-bootstrap/Tab";
import Tabs from 'react-bootstrap/Tabs';
const Why = () => {
	const defaultTab = "client";
	const [key, setKey] = useState(defaultTab);

	return (
        <div style={styles.root}>
			<Tabs
				defaultActiveKey={ defaultTab }
				id="tabs"
				activeKey={ key }
				onSelect={(k) => k && setKey(k)}
				className="my-tabs mb-3"
				transition={false}  fill
			>
				<Tab eventKey="client" title="client.dll">
					<DisplayOffsets data={clientJson} title="client_dll" />;
				</Tab>
				<Tab eventKey="server" title="server.dll">
					<DisplayOffsets data={serverJson} title="server_dll" />;
				</Tab>
				<Tab eventKey="offsets" title="offsets.dll">
					<DisplayOffsets data={offsetsJson} title="offsets" />;
				</Tab>
			</Tabs>

		</div>
    );
}

const styles: Record<"root" | "title" | "card", CSSProperties> = {
    root: {
        fontSize: "1.5rem",
        padding: 12,
		color: "lightsteelblue",

    },
    title: {
        fontSize: "2rem",
        margin: 0,
        color: "gold",
		textShadow: "1px 2px #B59410",
		textAlign: "center"
    },
	card: {
		maxWidth: 800,
		margin: "16px auto 0",
		padding: 12,
		borderRadius: 8,
		display: "flex",
		flexDirection: "column",
		justifyContent: "center",
	}
};
export default Why;
