import type { ReactNode, CSSProperties } from "react";
import Tab from 'react-bootstrap/Tab';
import Tabs from 'react-bootstrap/Tabs';
import React, { useState } from "react";

/*
* client.dll, server.dll, offsets.dll, buttons.dll
*
* encapsulate data via layer
* */
const RenderTextOffsets = () => {
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
					Tab content for Client
				</Tab>
				<Tab eventKey="server" title="server.dll">
					Tab content for Server
				</Tab>
				<Tab eventKey="offsets" title="offsets.dll">
					Tab content for Offsets
				</Tab>
				<Tab eventKey="buttons" title="buttons.dll">
					Tab content for Buttons
				</Tab>
			</Tabs>

		</div>
	);
}

const styles: Record<"root", CSSProperties> = {
	root: {
		padding: 12
	},

};
export default RenderTextOffsets;
