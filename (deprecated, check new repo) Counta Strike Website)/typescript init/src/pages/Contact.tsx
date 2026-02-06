import type { ReactNode, CSSProperties } from "react";

const Contact = () => {
    return (
        <div style={styles.root}>
            this is going to be within the contact info
        </div>
    );
}

const styles: Record<"root" | "title", CSSProperties> = {
    root: { padding: 12 },
    title: { margin: 0 },
};
export default Contact;
